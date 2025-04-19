/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marouane <marouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:27:49 by msaadaou          #+#    #+#             */
/*   Updated: 2025/04/19 19:28:04 by marouane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

typedef struct s_node
{
	char	*key;
	char	*value;
	int		sit;
} t_node;

typedef struct s_hashmap
{
	t_node *arr[4];
	size_t	size[4];
	size_t	element_size[4];
	float	percent[4];	
} t_hashmap;

size_t Jenkins_one_at_a_time_hash(char *str)
{
    size_t hash, i;
    for (hash = i = 0; str[i] != '\0'; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}


size_t	hash_function(char *str)
{
	size_t hash = 5381;

	while (*str)
	{
		hash = ((hash << 5) + hash) + *str;
		str++;
	}
	return hash;
}

void	hashmap_init(t_hashmap *map, size_t size)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		map->arr[i] = calloc(size , sizeof(t_node));
		map->element_size[i] = 0;
		map->percent[i] = 0;
		map->size[i] = size;
		i++;
	}
}

void	resize_bucket(t_hashmap *map, int index)
{
	size_t	new_size;
	t_node	*new_bucket;
	char	*key;
	char	*value;
	size_t	new_index;
	size_t	i;
	
	new_size = map->size[index] * 2;
	new_bucket = calloc(sizeof(t_node), new_size);
	if (!new_bucket)
		return ;
	i = 0;
	while (i < map->size[index])
	{
		if (map->arr[index][i].sit == 1)
		{
			key = map->arr[index][i].key;
			value = map->arr[index][i].value;
			new_index = hash_function(key) % new_size;
			while (new_bucket[new_index].sit)
				new_index = (new_index + 1) % new_size;
			new_bucket[new_index].key = key;
			new_bucket[new_index].value = value;
			new_bucket[new_index].sit = 1;
		}
		i++;
	}
	free(map->arr[index]);
	map->arr[index] = new_bucket;
	map->size[index] = new_size;
}

void	check_resize(t_hashmap *map, int index)
{
	map->percent[index] = (float)map->element_size[index] / map->size[index];
	if (map->percent[index] > 0.7)
		resize_bucket(map, index);
}

void	insert_map(t_hashmap *map, char *key, char *value)
{
	size_t	index1;
	size_t	index2;

	index1 = Jenkins_one_at_a_time_hash(key) % 4;
	check_resize(map, index1);
	index2 = hash_function(key) % map->size[index1];
	// int counter = 0;
	if (!map->arr[index1][index2].sit)
	{
		map->arr[index1][index2].key = key;
		map->arr[index1][index2].value = value;
		map->arr[index1][index2].sit = 1;
		map->element_size[index1]++;
		return ;
	}
	while (index2 < map->size[index1])
	{
		if (!map->arr[index1][index2].sit || !strcmp(key, map->arr[index1][index2].key))
		{
			if (!map->arr[index1][index2].sit)
				map->element_size[index1]++;
			map->arr[index1][index2].key = key;
			map->arr[index1][index2].value = value;
			map->arr[index1][index2].sit = 1;
			return ;
		}
		index2++;
		if (index2 == map->size[index1])
			index2 = 0;
	}
}

char	*find_map(t_hashmap *map, char *key)
{
	size_t	index1;
	size_t	index2;

	index1 = Jenkins_one_at_a_time_hash(key) % 4;
	index2 = hash_function(key) % map->size[index1];
	while (index2 < map->size[index1])
	{
		if (map->arr[index1][index2].sit == 0)
			return (NULL);
		if (!strcmp(key, map->arr[index1][index2].key))
			return (map->arr[index1][index2].value);
		index2++;
		if (index2 == map->size[index1])
			index2 = 0;
	}
	return (NULL);
}

void ft_putstr(char *str, int flag)
{
	size_t	i;
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n' && flag)
			flag = 0;
		else
			write(1, &str[i], 1);
		i++;
	}
}

char *get_next_line(int fd);

void	parsing_data(t_hashmap *map)
{
	char	*key;
	char	*value;
	size_t	insert_count = 0;

	key = get_next_line(0);
	while (key && key[0] != '\n')
	{
		value = get_next_line(0);
		if (!value)
			break;
		insert_map(map, key, value);
		key = get_next_line(0);
		insert_count++;
		if (insert_count % 1000 == 0)
			printf("%zu...\n", insert_count);
	}
	key = get_next_line(0);
	while (key)
	{
		char *result = find_map(map, key);
		if (result)
			ft_putstr(result, 0);
		else
		{
			ft_putstr(key, 1);
			ft_putstr(": Not found.\n", 0);
		}
		key = get_next_line(0);
	}
}


int main()
{
	t_hashmap	map;
	hashmap_init(&map, 1000000);
	parsing_data(&map);
	free(map.arr[0]);
	free(map.arr[1]);
	free(map.arr[2]);
	free(map.arr[3]);
	return (0);
}