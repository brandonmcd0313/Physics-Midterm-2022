#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// Function to check if a key has been pressed
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

int main(void)
{
  // Initialize wiringPi and set the 1st pin as input
  wiringPiSetup();
  pinMode(1, INPUT);

  while (1)
  {
    // Read the value of the pin and print it
    int value = digitalRead(1);
    printf("GPIO 1 has value: %d\n", value);

    // Check if the 'q' key has been pressed
    if (kbhit())
    {
      char c = getchar();
      if (c == 'q')
      {
        break;
      }
    }

    // Wait for a second before reading the value again
    delay(1000);
  }

  return 0;
}