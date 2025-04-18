/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadaou <msaadaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:27:49 by msaadaou          #+#    #+#             */
/*   Updated: 2025/04/18 20:31:36 by msaadaou         ###   ########.fr       */
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
	size_t	index;

	index = hash_function(key) % map->size;
	if (!map->arr[index].sit)
	{
		map->arr[index].key = key;
		map->arr[index].value = value;
		map->arr[index].sit = 1;
		return ;
	}
	while (index < map->size)
	{
		if (!map->arr[index].sit || !strcmp(key, map->arr[index].key))
		{
			map->arr[index].key = key;
			map->arr[index].value = value;
			map->arr[index].sit = 1;
			return ;
		}
		index++;
		if (index == map->size)
			index = 0;
	}
}

char	*find_map(t_hashmap *map, char *key)
{
	size_t	index;

	index = hash_function(key) % map->size;
	while (index < map->size)
	{
		if (!strcmp(key, map->arr[index].key))
			return (map->arr[index].value);
		if (map->arr[index].sit == 0)
			return (NULL);
		index++;
		if (index == map->size)
			index = 0;
	}
	return (NULL);
}

char *get_next_line(int fd);

void parsing_data(t_hashmap *map)
{
	char 	*key;
	char	*value;

	key = get_next_line(0);
	value = get_next_line(0);
	size_t	i = 0;
	while (value)
	{
		insert_map(map, key, value);
		key = get_next_line(0);
		if (key[0] == '\n')
		{
			break;
		}
		value = get_next_line(0);
		i++;
	}
	key = get_next_line(0);
	while (key)
	{
		printf("%s", find_map(map, key));
		key = get_next_line(0);
	}
}

int main()
{
	t_hashmap	map;
	hashmap_init(&map, 25000);
	parsing_data(&map);
	free(map.arr);
	return (0);
}