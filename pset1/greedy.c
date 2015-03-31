#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
float change_input;
int coin_count = 0;

printf("Input the change owed:\n");

do
{
change_input = GetFloat();
if(change_input <= 0)
{
printf("Try again! Change owed must be positive:\n");
}
}
while (change_input <= 0);

int change_cents = roundf((change_input * 100));

while (change_cents >= 25)
{
change_cents -= 25;
coin_count++;
}
while (change_cents >= 10)
{
change_cents -= 10;
coin_count++;
}
while (change_cents >= 5)
{
change_cents -= 5;
coin_count++;
}
while (change_cents >= 1)
{
change_cents -= 1;
coin_count++;
}

printf("%d\n",coin_count);

}
