#include "mapreduce.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


unsigned long MR_DefaultHashPartition(char *key, int num_partitions) 
{
	unsigned long hash = 5381;
	int c;
	while ((c = *key++) != '\0')
	    hash = hash * 33 + c;
	return hash % num_partitions;
}