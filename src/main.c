/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jde-meo <jde-meo@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 16:13:32 by jde-meo           #+#    #+#             */
/*   Updated: 2023/10/19 15:37:01 by jde-meo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *flags = "-0# +";
#define DEF "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_PURPLE "\033[1;35m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_WHITE "\033[1;37m"
#define setColor(x) printf(x)

int urandom_fd = -2;

static void	urandom_init() 
{
  urandom_fd = open("/dev/urandom", O_RDONLY);

  if (urandom_fd == -1) {
    int errsv = urandom_fd;
    printf("Error opening [/dev/urandom]: %i\n", errsv);
    exit(1);
  }
}

static unsigned long urandom() 
{
  unsigned long buf_impl;
  unsigned long *buf = &buf_impl;

  if (urandom_fd == -2) {
    urandom_init();
  }

  /* Read sizeof(long) bytes (usually 8) into *buf, which points to buf_impl */
  read(urandom_fd, buf, sizeof(long));
  return buf_impl;
}

static unsigned int	strlcpy(char *dest, char *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	i2;

	i = 0;
	i2 = 0;
	while (src[i] != '\0')
		i++;
	if (size > 0)
	{
		while (src[i2] != '\0' && i2 < size - 1)
		{
			dest[i2] = src[i2];
			i2++;
		}
		dest[i2] = '\0';
	}
	return (i);
}

void	shuffle_str(char *str)
{
	int	len = strlen(str);
	for (int i = 0; i++ < len * len;)
	{
		int	i1 = urandom() % len;
		int i2 = urandom() % len;
		char temp = str[i1];
		str[i1] = str[i2];	
		str[i2] = temp;
	}
}

static size_t	strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	dest_length;
	size_t	src_length;

	src_length = strlen(src);
	dest_length = strlen(dst);
	j = dest_length;
	i = 0;
	if (dest_length < size - 1 && size > 0)
	{
		while (src[i] && dest_length + i < size - 1)
		{
			dst[j] = src[i];
			j++;
			i++;
		}
		dst[j] = 0;
	}
	if (dest_length >= size)
		dest_length = size;
	return (dest_length + src_length);
}

static char	*func_straddchr(char *str, char c)
{
	char	*out;
	int		len;

	len = 0;
	if (str != NULL)
		len = strlen(str);
	out = (char *)malloc(sizeof(char) * (len + 2));
	if (out == NULL)
		return (NULL);
	if (str != NULL)
		strlcpy(out, (char *)str, len + 1);
	out[len] = c;
	out[len + 1] = '\0';
	if (str != NULL)
		free(str);
	return (out);
}

static char	*strcat_malloc(char *dest, char *src)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	if (dest)
		len1 = strlen(dest);
	len2 = strlen(src);
	str = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (!str)
		return (NULL);
	*str = '\0';
	if (dest)
	{
		strlcpy(str, dest, len1 + 1);
		free(dest);
	}
	strlcat(str, src, len1 + len2 + 1);
	return (str);
}

static void	itoa_base_proc(char **str, long long int nb, char *base)
{
	int	base_len;

	base_len = strlen(base);
	if (nb < 0)
		nb *= -1;
	if (nb >= base_len)
		itoa_base_proc(str, nb / base_len, base);
	*str = func_straddchr(*str, base[nb % base_len]);
}

static char	*itoa_base_ft(long long int n, char *base, int ptr)
{
	char	*str;

	str = NULL;
	itoa_base_proc(&str, n, base);
	return (str);
}

static char	*gen_random_str(int size)
{
	int		len = urandom() % 20;
	if (size > 0)
		len = size;
	char	*str = (char *)malloc(sizeof(char) * (len + 1));
	str[len] = '\0';
	while (--len >= 0)
		str[len] = urandom() % 255;
	return (str);
}

static void	gen_random_format(char **str, char spec)
{
	*str = func_straddchr(*str, '|');
	*str = func_straddchr(*str, '%');
	int size = urandom() % 6;
	for (int i = 0; i < size; i++)
	{
		*str = func_straddchr(*str, flags[urandom() % 5]);
	}
	char *itoaout = itoa_base_ft(urandom() % 30, "0123456789", 0);
	*str = strcat_malloc(*str, itoaout);
	free(itoaout);
	*str = func_straddchr(*str, '.');
		  itoaout = itoa_base_ft(urandom() % 30, "0123456789", 0);
	*str = strcat_malloc(*str, itoaout);
	free(itoaout);
	*str = func_straddchr(*str, spec);
}

int	main(int ac, char **av)
{
	int chr_test = 0;
	int str_test = 0;
	int ptr_test = 0;
	int	int_test = 0;
	int	hex_test = 0;
	int prc_test = 0;
	int	bonus_test = 0;

	for (int i = 1; i < ac; i++)
	{
		if (av[i][0] == 'c')
		{
			chr_test = 1;
		}
		else if (av[i][0] == 's')
		{
			str_test = atoi(av[i] + 1);
		}
		else if (av[i][0] == 'p')
		{
			ptr_test = atoi(av[i] + 1);
		}
		else if (av[i][0] == 'd' || av[i][0] == 'i' || av[i][0] == 'u')
		{
			int_test = atoi(av[i] + 1);
		}
		else if (av[i][0] == 'x' || av[i][0] == 'X')
		{
			hex_test = atoi(av[i] + 1);
		}
		else if (av[i][0] == '%')
		{
			prc_test = atoi(av[i] + 1);
		}
		else if (av[i][0] == 'b')
		{
			bonus_test = atoi(av[i] + 1);
		}
	}

	if ((chr_test + str_test + ptr_test + int_test + hex_test + prc_test + bonus_test) >= 340)
	{
		setColor(BOLD_RED);
		printf("Too much tests (can't exceed 339 tests)\n");
		setColor(RED);
		printf("Aborting...\n");
		return (-1);
	}

	urandom_init();
	/*
					CHAR
	*/
	if (chr_test)
	{
		setColor(BOLD_CYAN);
		printf("|	CHAR TESTS 	|\n");
		char *format = NULL;
		char *s = NULL;
		for	(int i = 0; i < 256; i++)
		{
			format = strcat_malloc(format, "%c");
			s = func_straddchr(s, (char)i);
		}
		shuffle_str(s);
		int	stdout_bk = dup(fileno(stdout));
		int	pipefd[2];
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		
		int res1 = ft_printf(format, s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15], s[16], s[17], s[18], s[19], s[20], s[21], s[22], s[23], s[24], s[25], s[26], s[27], s[28], s[29], s[30], s[31], s[32], s[33], s[34], s[35], s[36], s[37], s[38], s[39], s[40], s[41], s[42], s[43], s[44], s[45], s[46], s[47], s[48], s[49], s[50], s[51], s[52], s[53], s[54], s[55], s[56], s[57], s[58], s[59], s[60], s[61], s[62], s[63], s[64], s[65], s[66], s[67], s[68], s[69], s[70], s[71], s[72], s[73], s[74], s[75], s[76], s[77], s[78], s[79], s[80], s[81], s[82], s[83], s[84], s[85], s[86], s[87], s[88], s[89], s[90], s[91], s[92], s[93], s[94], s[95], s[96], s[97], s[98], s[99], s[100], s[101], s[102], s[103], s[104], s[105], s[106], s[107], s[108], s[109], s[110], s[111], s[112], s[113], s[114], s[115], s[116], s[117], s[118], s[119], s[120], s[121], s[122], s[123], s[124], s[125], s[126], s[127], s[128], s[129], s[130], s[131], s[132], s[133], s[134], s[135], s[136], s[137], s[138], s[139], s[140], s[141], s[142], s[143], s[144], s[145], s[146], s[147], s[148], s[149], s[150], s[151], s[152], s[153], s[154], s[155], s[156], s[157], s[158], s[159], s[160], s[161], s[162], s[163], s[164], s[165], s[166], s[167], s[168], s[169], s[170], s[171], s[172], s[173], s[174], s[175], s[176], s[177], s[178], s[179], s[180], s[181], s[182], s[183], s[184], s[185], s[186], s[187], s[188], s[189], s[190], s[191], s[192], s[193], s[194], s[195], s[196], s[197], s[198], s[199], s[200], s[201], s[202], s[203], s[204], s[205], s[206], s[207], s[208], s[209], s[210], s[211], s[212], s[213], s[214], s[215], s[216], s[217], s[218], s[219], s[220], s[221], s[222], s[223], s[224], s[225], s[226], s[227], s[228], s[229], s[230], s[231], s[232], s[233], s[234], s[235], s[236], s[237], s[238], s[239], s[240], s[241], s[242], s[243], s[244], s[245], s[246], s[247], s[248], s[249], s[250], s[251], s[252], s[253], s[254], s[255]);
	
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer1[res1 + 1];
  		read(pipefd[0], buffer1, res1);
		buffer1[res1] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));

		int res2 = printf(format, s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15], s[16], s[17], s[18], s[19], s[20], s[21], s[22], s[23], s[24], s[25], s[26], s[27], s[28], s[29], s[30], s[31], s[32], s[33], s[34], s[35], s[36], s[37], s[38], s[39], s[40], s[41], s[42], s[43], s[44], s[45], s[46], s[47], s[48], s[49], s[50], s[51], s[52], s[53], s[54], s[55], s[56], s[57], s[58], s[59], s[60], s[61], s[62], s[63], s[64], s[65], s[66], s[67], s[68], s[69], s[70], s[71], s[72], s[73], s[74], s[75], s[76], s[77], s[78], s[79], s[80], s[81], s[82], s[83], s[84], s[85], s[86], s[87], s[88], s[89], s[90], s[91], s[92], s[93], s[94], s[95], s[96], s[97], s[98], s[99], s[100], s[101], s[102], s[103], s[104], s[105], s[106], s[107], s[108], s[109], s[110], s[111], s[112], s[113], s[114], s[115], s[116], s[117], s[118], s[119], s[120], s[121], s[122], s[123], s[124], s[125], s[126], s[127], s[128], s[129], s[130], s[131], s[132], s[133], s[134], s[135], s[136], s[137], s[138], s[139], s[140], s[141], s[142], s[143], s[144], s[145], s[146], s[147], s[148], s[149], s[150], s[151], s[152], s[153], s[154], s[155], s[156], s[157], s[158], s[159], s[160], s[161], s[162], s[163], s[164], s[165], s[166], s[167], s[168], s[169], s[170], s[171], s[172], s[173], s[174], s[175], s[176], s[177], s[178], s[179], s[180], s[181], s[182], s[183], s[184], s[185], s[186], s[187], s[188], s[189], s[190], s[191], s[192], s[193], s[194], s[195], s[196], s[197], s[198], s[199], s[200], s[201], s[202], s[203], s[204], s[205], s[206], s[207], s[208], s[209], s[210], s[211], s[212], s[213], s[214], s[215], s[216], s[217], s[218], s[219], s[220], s[221], s[222], s[223], s[224], s[225], s[226], s[227], s[228], s[229], s[230], s[231], s[232], s[233], s[234], s[235], s[236], s[237], s[238], s[239], s[240], s[241], s[242], s[243], s[244], s[245], s[246], s[247], s[248], s[249], s[250], s[251], s[252], s[253], s[254], s[255]);
		
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer2[res2 + 1];
  		read(pipefd[0], buffer2, res2);
		buffer2[res2] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));

		free(s);
		if (res1 == res2 && strcmp(buffer1, buffer2) == 0)
		{
			setColor(BOLD_GREEN);
			printf("[OK]");
			setColor(GREEN);
			printf(" for Chars 0 to 255\n");
		}
		else
		{
			setColor(BOLD_RED);
			printf("[KO]");
			setColor(RED);
			printf(" using format : \"%s\"\n", format);
			printf("   printf printed :|%s|", buffer2);
			printf("\nreturning : %d\n", res2);
			printf("ft_printf printed :|%s|", buffer1);
			printf("\nreturning : %d\n", res1);
		}
		free(format);
	}

	/*
					STRINGS
	*/
	int	str_pass = 1;
	setColor(BOLD_CYAN);
	if (str_test) printf("|	STRING TESTS 	|\n");
	for (int i = 0; i < str_test; i++)
	{
		char *format = NULL;
		char **strs = malloc(sizeof(char *) * 5);
		for (int i = 0; i < 5; i++)
		{
			strs[i] = gen_random_str(0);
			format = strcat_malloc(format, "|%s");
		}
		format = func_straddchr(format, '|');

		int	stdout_bk = dup(fileno(stdout));
		int	pipefd[2];
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res1 = ft_printf(format, strs[0], strs[1], strs[2], strs[3], strs[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer1[res1 + 1];
  		read(pipefd[0], buffer1, res1);
		buffer1[res1] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res2 = printf(format, strs[0], strs[1], strs[2], strs[3], strs[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer2[res2 + 1];
  		read(pipefd[0], buffer2, res2);
		buffer2[res2] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		if (!(res1 == res2 && strcmp(buffer1, buffer2) == 0))
		{
			str_pass = 0;
			setColor(BOLD_RED);
			printf("[KO]");
			setColor(RED);
			printf(" using format : \"%s\"\n", format);
			printf("   printf printed :|%s|", buffer2);
			printf("\nreturning : %d\n", res2);
			printf("ft_printf printed :|%s|", buffer1);
			printf("\nreturning : %d\n", res1);
		}

		free(format);
		for (int i = 0; i < 5; i++)
		{
			free(strs[i]);
		}
		free(strs);
	}
	if (str_pass && str_test)
	{
		setColor(BOLD_GREEN);
		printf("[OK]");
		setColor(GREEN);
		printf(" Succesfully passed %d random tests !\n", str_test);
	}

	/*
					POINTERS
	*/
	int	ptr_pass = 1;
	setColor(BOLD_CYAN);
	if (ptr_test) printf("|	POINTER TESTS 	|\n");
	for (int i = 0; i < ptr_test; i++)
	{
		char *format = NULL;
		void	**ptrs = malloc(sizeof(void *) * 5);
		for (int k = 0; k < 5; k++)
		{
			ptrs[k] = (void *)((unsigned long long)urandom() * (unsigned long long)urandom());
			format = strcat_malloc(format, "|%p");
		}
		format = func_straddchr(format, '|');

		int	stdout_bk = dup(fileno(stdout));
		int	pipefd[2];
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res1 = ft_printf(format, ptrs[0], ptrs[1], ptrs[2], ptrs[3], ptrs[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer1[res1 + 1];
  		read(pipefd[0], buffer1, res1);
		buffer1[res1] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res2 = printf(format, ptrs[0], ptrs[1], ptrs[2], ptrs[3], ptrs[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer2[res2 + 1];
  		read(pipefd[0], buffer2, res2);
		buffer2[res2] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		if (!(res1 == res2 && strcmp(buffer1, buffer2) == 0))
		{
			ptr_pass = 0;
			setColor(BOLD_RED);
			printf("[KO]");
			setColor(RED);
			printf(" using format : \"%s\"\n", format);
			printf("   printf printed :|%s|", buffer2);
			printf("\nreturning : %d\n", res2);
			printf("ft_printf printed :|%s|", buffer1);
			printf("\nreturning : %d\n", res1);
		}

		free(format);
		free(ptrs);
	}
	if (ptr_pass && ptr_test)
	{
		setColor(BOLD_GREEN);
		printf("[OK]");
		setColor(GREEN);
		printf(" Succesfully passed %d random tests !\n", ptr_test);
	}

	/*
					INTEGERS
	*/
	int	int_pass = 1;
	setColor(BOLD_CYAN);
	if (int_test) printf("|	INTEGER TESTS 	|\n");
	for (int i = 0; i < int_test; i++)
	{
		char *format = NULL;
		long int	*ints = malloc(sizeof(long int) * 5);
		for (int k = 0; k < 5; k++)
		{
			ints[k] = (long)urandom() - (long)urandom();
			format = strcat_malloc(format, "|%d");
		}
		for (int k = 0; k < 5; k++)
		{
			format = strcat_malloc(format, "|%i");
		}
		for (int k = 0; k < 5; k++)
		{
			format = strcat_malloc(format, "|%u");
		}
		format = func_straddchr(format, '|');

		int	stdout_bk = dup(fileno(stdout));
		int	pipefd[2];
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res1 = ft_printf(format, ints[0], ints[1], ints[2], ints[3], ints[4], ints[0], ints[1], ints[2], ints[3], ints[4], ints[0], ints[1], ints[2], ints[3], ints[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer1[res1 + 1];
  		read(pipefd[0], buffer1, res1);
		buffer1[res1] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res2 = printf(format, ints[0], ints[1], ints[2], ints[3], ints[4], ints[0], ints[1], ints[2], ints[3], ints[4], ints[0], ints[1], ints[2], ints[3], ints[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer2[res2 + 1];
  		read(pipefd[0], buffer2, res2);
		buffer2[res2] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		if (!(res1 == res2 && strcmp(buffer1, buffer2) == 0))
		{
			int_pass = 0;
			setColor(BOLD_RED);
			printf("[KO]");
			setColor(RED);
			printf(" using format : \"%s\"\n", format);
			printf("   printf printed :|%s|", buffer2);
			printf("\nreturning : %d\n", res2);
			printf("ft_printf printed :|%s|", buffer1);
			printf("\nreturning : %d\n", res1);
		}

		free(format);
		free(ints);
	}
	if (int_pass && int_test)
	{
		setColor(BOLD_GREEN);
		printf("[OK]");
		setColor(GREEN);
		printf(" Succesfully passed %d random tests !\n", int_test);
	}

	/*
					HEXADECIMAL
	*/
	int	hex_pass = 1;
	setColor(BOLD_CYAN);
	if (hex_test) printf("|	HEXA TESTS 	|\n");
	for (int i = 0; i < hex_test; i++)
	{
		char *format = NULL;
		long int	*ints = malloc(sizeof(long int) * 5);
		for (int k = 0; k < 5; k++)
		{
			ints[k] = (long)urandom() - (long)urandom();
			format = strcat_malloc(format, "|%x");
		}
		for (int k = 0; k < 5; k++)
		{
			format = strcat_malloc(format, "|%X");
		}
		format = func_straddchr(format, '|');

		int	stdout_bk = dup(fileno(stdout));
		int	pipefd[2];
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res1 = ft_printf(format, ints[0], ints[1], ints[2], ints[3], ints[4], ints[0], ints[1], ints[2], ints[3], ints[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer1[res1 + 1];
  		read(pipefd[0], buffer1, res1);
		buffer1[res1] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res2 = printf(format, ints[0], ints[1], ints[2], ints[3], ints[4], ints[0], ints[1], ints[2], ints[3], ints[4]);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer2[res2 + 1];
  		read(pipefd[0], buffer2, res2);
		buffer2[res2] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		if (!(res1 == res2 && strcmp(buffer1, buffer2) == 0))
		{
			hex_pass = 0;
			setColor(BOLD_RED);
			printf("[KO]");
			setColor(RED);
			printf(" using format : \"%s\"\n", format);
			printf("   printf printed :|%s|", buffer2);
			printf("\nreturning : %d\n", res2);
			printf("ft_printf printed :|%s|", buffer1);
			printf("\nreturning : %d\n", res1);
		}

		free(format);
		free(ints);
	}
	if (hex_pass && hex_test)
	{
		setColor(BOLD_GREEN);
		printf("[OK]");
		setColor(GREEN);
		printf(" Succesfully passed %d random tests !\n", hex_test);
	}

	/*
					%%%%%%%%%%%
	*/
	if (prc_test) 
	{
		setColor(BOLD_CYAN);
		printf("|	%% TESTS 	|\n");
		char *format = NULL;
		for (int k = 0; k < prc_test; k++)
		{
			int pcam = (urandom() % 125) + 1;
			format = func_straddchr(format, '|');
			while(--pcam > 0)
				format = func_straddchr(format, '%');
		}
		format = func_straddchr(format, '|');

		int	stdout_bk = dup(fileno(stdout));
		int	pipefd[2];
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res1 = ft_printf(format, 0);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer1[res1 + 1];
  		read(pipefd[0], buffer1, res1);
		buffer1[res1] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res2 = printf(format, 0);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer2[res2 + 1];
  		read(pipefd[0], buffer2, res2);
		buffer2[res2] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		if (res1 == res2 && strcmp(buffer1, buffer2) == 0)
		{
			setColor(BOLD_GREEN);
			printf("[OK]");
			setColor(GREEN);
			printf(" Succesfully passed %d random tests !\n", prc_test);
		}
		else
		{
			hex_pass = 0;
			setColor(BOLD_RED);
			printf("[KO]");
			setColor(RED);
			printf(" using format : \"%s\"\n", format);
			printf("   printf printed :|%s|", buffer2);
			printf("\nreturning : %d\n", res2);
			printf("ft_printf printed :|%s|", buffer1);
			printf("\nreturning : %d\n", res1);
		}

		free(format);
	}

	/*
					BONUS
	*/
	int bonus_pass = 1;
	setColor(BOLD_CYAN);
	if (bonus_test) printf("|	BONUS TESTS 	|\n");
	for (int i = 0; i < bonus_test; i++)
	{
		char *format = NULL;
		gen_random_format(&format, 'c');
		gen_random_format(&format, 's');
		gen_random_format(&format, 'p');
		gen_random_format(&format, 'd');
		gen_random_format(&format, 'i');
		gen_random_format(&format, 'u');
		gen_random_format(&format, 'x');
		gen_random_format(&format, 'X');
		gen_random_format(&format, '%');
		format = func_straddchr(format, '|');
		int	randomint = urandom() % 4096;
		unsigned long long pointer = urandom() % LLONG_MAX;
		char *stringtest = gen_random_str(0);
		char chartest = urandom() % 125 + 1;

		int	stdout_bk = dup(fileno(stdout));
		int	pipefd[2];
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res1 = printf(format, chartest, stringtest, pointer, randomint, -randomint, randomint - 255, randomint - 2048, randomint - 4096);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer1[res1 + 1];
  		read(pipefd[0], buffer1, res1);
		buffer1[res1] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		pipe(pipefd);
		// REDIRECT STDOUT TO PIPE ===========
		dup2(pipefd[1], fileno(stdout));
		int res2 = ft_printf(format, chartest, stringtest, pointer, randomint, -randomint, randomint - 255, randomint - 2048, randomint - 4096);
		fflush(stdout);
		write(pipefd[1], "\0", 1);
		close(pipefd[1]);
		char buffer2[res2 + 1];
  		read(pipefd[0], buffer2, res2);
		buffer2[res2] = '\0';
		// RESTORE STDOUT ====================
		dup2(stdout_bk, fileno(stdout));
		
		if (!(res1 == res2 && memcmp(buffer1, buffer2, res1) == 0))
		{
			bonus_pass = 0;
			setColor(BOLD_RED);
			printf("[KO]");
			setColor(RED);
			printf(" using format : \"%s\"\n", format);
			printf("ft_printf printed :%s\n", buffer2);
			printf("returning : %d\n", res2);
			printf("   printf printed :%s\n", buffer1);
			printf("returning : %d\n", res1);
		}
		free(stringtest);
		free(format);
	}
	if (bonus_pass && bonus_test)
	{
		setColor(BOLD_GREEN);
		printf("[OK]");
		setColor(GREEN);
		printf(" Succesfully passed %d random tests !\n", bonus_test);
	}
}
