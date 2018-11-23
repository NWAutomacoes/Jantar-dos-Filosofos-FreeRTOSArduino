#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define qtyFilo 10

SemaphoreHandle_t xGarfosSem[qtyFilo];
SemaphoreHandle_t xSerial;

void TaskFilosofos(void *pvParameters);

void setup()
{
  Serial.begin(9600);

  for (int x = 0; x < qtyFilo; x++)
  {
    xGarfosSem[x] = xSemaphoreCreateMutex();
  }
  xSerial = xSemaphoreCreateMutex();
  for (int x = 0; x < 10; x++)
  {
    xTaskCreate(TaskFilosofos, (const portCHAR *)"Filosofos", 128, (void *)x, 1, NULL);
  }
}
void loop()
{
}

void TaskFilosofos(void *parameter)
{
  int x, x2;
  x = (int)parameter;

  if (x == (qtyFilo - 1))
  {
    x2 = 0;
  }
  else
    x2 = x + 1;

  //(void)pvParameters;
  while (1)
  {
    if (xSemaphoreTake(xGarfosSem[x], (TickType_t)0) == pdTRUE)
    {
      if (xSemaphoreTake(xGarfosSem[x2], (TickType_t)0) == pdTRUE)
      {
        if (xSemaphoreTake(xSerial, (TickType_t)portMAX_DELAY) == pdTRUE)
        {
          Serial.print("Filosofo ");
          Serial.print(x);
          Serial.println(" esta comendo///////////////////////////////");
          xSemaphoreGive(xSerial);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for one second
        //vTaskDelay(random(1000, 10000) / portTICK_PERIOD_MS);
        xSemaphoreGive(xGarfosSem[x]);
        xSemaphoreGive(xGarfosSem[x2]);
        
      }
      else
        xSemaphoreGive(xGarfosSem[x]);
    }
    if (xSemaphoreTake(xSerial, (TickType_t)portMAX_DELAY) == pdTRUE)
    {
      Serial.print("Filósofo ");
      Serial.print(x);
      Serial.println(" esta pensando");
      xSemaphoreGive(xSerial);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS); // wait for one second
  }
}
