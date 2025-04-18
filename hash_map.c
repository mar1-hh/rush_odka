/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marouane <marouane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:27:49 by msaadaou          #+#    #+#             */
/*   Updated: 2025/04/18 23:30:19 by marouane         ###   ########.fr       */
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
	size_t	size;
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
	map->arr[0] = calloc(size , sizeof(t_node));
	map->arr[1] = calloc(size , sizeof(t_node));
	map->arr[2] = calloc(size , sizeof(t_node));
	map->arr[3] = calloc(size , sizeof(t_node));
	map->size = size;
}

void	insert_map(t_hashmap *map, char *key, char *value)
{
	size_t	index1;
	size_t	index2;

	index1 = Jenkins_one_at_a_time_hash(key) % 4;
	index2 = hash_function(key) % map->size;
	if (!map->arr[index1][index2].sit)
	{
		map->arr[index1][index2].key = key;
		map->arr[index1][index2].value = value;
		map->arr[index1][index2].sit = 1;
		return ;
	}
	while (index2 < map->size)
	{
		if (!map->arr[index1][index2].sit || !strcmp(key, map->arr[index1][index2].key))
		{
			map->arr[index1][index2].key = key;
			map->arr[index1][index2].value = value;
			map->arr[index1][index2].sit = 1;
			return ;
		}
		index2++;
		if (index2 == map->size)
			index2 = 0;
	}
}

char	*find_map(t_hashmap *map, char *key)
{
	size_t	index1;
	size_t	index2;

	index1 = Jenkins_one_at_a_time_hash(key) % 4;
	index2 = hash_function(key) % map->size;
	while (index2 < map->size)
	{
		if (map->arr[index1][index2].sit == 0)
			return (NULL);
		if (!strcmp(key, map->arr[index1][index2].key))
			return (map->arr[index1][index2].value);
		index2++;
		if (index2 == map->size)
			index2 = 0;
	}
	return (NULL);
}

char *get_next_line(int fd);

void	parsing_data(t_hashmap *map)
{
	char	*key;
	char	*value;

	key = get_next_line(0);
	while (key && key[0] != '\n')
	{
		value = get_next_line(0);
		if (!value)
			break;
		insert_map(map, key, value);
		key = get_next_line(0);
	}
	key = get_next_line(0);
	while (key)
	{
		char *result = find_map(map, key);
		if (result)
			printf("%s", result);
		else
			printf("%s: Not found.\n", key);
		key = get_next_line(0);
	}
}


int main()
{
	t_hashmap	map;
	hashmap_init(&map, 25000);
	parsing_data(&map);
	free(map.arr[0]);
	free(map.arr[1]);
	free(map.arr[2]);
	free(map.arr[3]);
	return (0);
}