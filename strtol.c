#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/* dummy implementation of strtol */

void error_endPtr_setter(const char *nPtr, char** endPtr, int i, int base)
{
	bool loop_var = true;
	int temp_int;
	while (loop_var)
	{
		if (nPtr[i] >= 'A' && nPtr[i] <= 'Z') // between A and Z
		{
			temp_int = 10 + nPtr[i] % 'A';
		}
		else if (nPtr[i] >= 'a' && nPtr[i] <= 'z') // between a and z
		{
			temp_int = 10 + nPtr[i] % 'z';
		}
		else if (nPtr[i] >= '0' && nPtr[i] <= '9') // between 0 and 9
		{
			temp_int = nPtr[i] % '0';
		}
		else
		{
			loop_var = false;
			break;
		}
		if (temp_int > base)
		{
			loop_var = false;
			break;
		}
		i++;
	}
	if (endPtr)
	{
		*endPtr = (char *)nPtr + i;
	}
}

long error_max(const char *nPtr, char** endPtr, int i, int base)
{
	errno = ERANGE;
	error_endPtr_setter(nPtr, endPtr, i, base);
	return LONG_MAX;
}

long error_min(const char *nPtr, char** endPtr, int i, int base)
{
	errno = ERANGE;
	error_endPtr_setter(nPtr, endPtr, i, base);
	return LONG_MIN;
}

int base_error()
{
	errno = EINVAL;
	return 0;
}

long strtol(const char *nPtr, char **endPtr, int base)
{
	int help_var_first = 0;
	int help_var_second = 0;
	bool is_negative = false;
	long number_value = 0;
	bool loop_var = true;
	long temp_int;
	bool is_empty = true;

	while (isspace(nPtr[help_var_first])) // ascii("spacebar") = 32
	{
		help_var_first++;
	}
	if (nPtr[help_var_first] == '-') // ascii("-") = 45
	{
		is_negative = true;
		help_var_first++;
	}
	if (nPtr[help_var_first] == '+')
	{
		help_var_first++;
	}
	if (base == 0)
	{
		if (nPtr[help_var_first] == '0' && (nPtr[help_var_first + 1] == 'x' || nPtr[help_var_first + 1] == 'X')) // 0 and x or X
		{
			base = 16;
			help_var_first += 2;
			help_var_second = help_var_first - 1;
		}
		else if (nPtr[help_var_first] == '0') // 0
		{
			base = 8;
			help_var_first += 1;
			help_var_second = help_var_first;
		}
		else if (nPtr[help_var_first] >= '1' && nPtr[help_var_first] <= '9') // number between 1 and 9
		{
			base = 10;
		}
		else
		{
			return base_error();
		}
	}
	if (base == 16 && nPtr[help_var_first] == '0' && (nPtr[help_var_first + 1] == 'x' || nPtr[help_var_first + 1] == 'X'))
	{
		help_var_first += 2;
		help_var_second = help_var_first - 1;
	}
	if (base == 8 && nPtr[help_var_first] == '0')
	{
		help_var_first += 1;
		help_var_second = help_var_first;
	}
	if (base == 1 || base < 0 || base > 36)
	{
		return base_error();
	}
	while (loop_var)
	{
		if (nPtr[help_var_first] >= 'A' && nPtr[help_var_first] <= 'Z')
		{
			temp_int = 10 + nPtr[help_var_first] % 'A';
		}
		else if (nPtr[help_var_first] >= 'a' && nPtr[help_var_first] <= 'z')
		{
			temp_int = 10 + nPtr[help_var_first] % 'a';
		}
		else if (nPtr[help_var_first] >= '0' && nPtr[help_var_first] <= '9')
		{
			temp_int = nPtr[help_var_first] % '0';
		}
		else
		{
			loop_var = false;
			break;
		}
		if (temp_int >= base)
		{
			loop_var = false;
			break;
		}
		if (is_negative)
		{
			if (LONG_MIN - (number_value * base - temp_int) > 0)
			{
				return error_min(nPtr, endPtr, help_var_first, base);
			}
			else if (number_value == LONG_MIN)
			{
				return error_min(nPtr, endPtr, help_var_first, base);
			}
			number_value = number_value * base - temp_int;
		}
		else
		{
			if (LONG_MAX - (number_value * base + temp_int) < 0)
			{
				return error_max(nPtr, endPtr, help_var_first, base);
			}
			else if (number_value == LONG_MAX)
			{
				return error_max(nPtr, endPtr, help_var_first, base);
			}
			number_value = number_value * base + temp_int;
		}
		help_var_first++;
		is_empty = false;
	}
	if (is_empty && endPtr)
	{
		*endPtr = (char *)nPtr + help_var_second;
	}
	else if (endPtr)
	{
		*endPtr = (char *)nPtr + help_var_first;
	}
	return number_value;
	if (endPtr)
	{
		*endPtr = (char *)nPtr;
	}
}
