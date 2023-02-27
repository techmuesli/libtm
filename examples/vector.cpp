//
// vector.cpp
//

#include <iostream>
#include <string.h>
#include "tm_vector.h"

typedef struct city_data_s
{
	char	*city;
	int		population;
}				city_data_t;

static void fill_test_data(city_data_t *data);
static void city_cleanup(void *data);

int	main(void)
{
	city_data_t		*city;
	city_data_t		test_data[10];
	tm_vector_t		*vector;

	vector = tm_vector_init(5, 5, sizeof(city_data_t));
	tm_vector_custom_cleanup(vector, city_cleanup);

	fill_test_data(test_data);
	tm_vector_add(vector, &test_data[0]);
	city = (city_data_t *)tm_vector_get(vector, 0);
	std::cout << "Name: " << city->city << ", Population: " << city->population << std::endl;

	for (int i = 1; i < 10; i++)
		tm_vector_add(vector, &test_data[i]);

	tm_vector_delete(vector, 0);

	for (size_t i = 0; i < tm_vector_size(vector); i++)
	{
		city = (city_data_t *)tm_vector_get(vector, i);
		std::cout << "Name: " << city->city << ", Population: " << city->population << std::endl;
	}

	tm_vector_cleanup(vector);

	return (0);
}

static void city_cleanup(void *data)
{
	city_data_t	*city = (city_data_t *)data;

	if (city->city != NULL)
	{
		free(city->city);
		city->city = NULL;
	}
}

static void fill_test_data(city_data_t *data)
{
	data[0].city = strdup("Berlin");
	data[0].population = 3677472;
	data[1].city = strdup("Hamburg");
	data[1].population = 1906411;
	data[2].city = strdup("Munich");
	data[2].population = 1487708;
	data[3].city = strdup("Cologne");
	data[3].population = 1073096;
	data[4].city = strdup("Franfurkt am Main");
	data[4].population = 759224;
	data[5].city = strdup("Stuttgart");
	data[5].population = 626275;
	data[6].city = strdup("Duesseldorf");
	data[6].population = 619477;
	data[7].city = strdup("Leipzig");
	data[7].population = 601866;
	data[8].city = strdup("Dortmund");
	data[8].population = 586852;
	data[9].city = strdup("Essen");
	data[9].population = 579432;
}
