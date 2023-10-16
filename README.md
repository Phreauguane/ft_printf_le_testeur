# ft_printf Le Testeur

this is my full tester for the 42 project ft_printf

it makes random tests and compares your output to the original printf one

# How to use

clone this repository to a new folder

copy your project into the new folder

go to ```grademe.sh``` and change ```-I ft_printf``` to the correct include path

for example ```-I ft_printf/include```

Make your libftprintf.a

then you can run grademe.sh

you can do the tests you want by using the command :

```./test.out ``` with the flags you want to test followed by the number of tests you want to perform

for example : ```./test.out b100 s20 x60``` will do 100 tests for the bonus, 20 for the strings (```%s``` flag)

and 60 tests for the hexadecimal integers
