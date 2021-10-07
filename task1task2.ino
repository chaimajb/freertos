#include <Arduino_FreeRTOS.h>

//define task handles
TaskHandle_t FirstTask_Handler;
TaskHandle_t SecondTask_Handler;

// define two tasks 
void FirstTask( void *pvParameters );
void SecondTask(void* pvParameters);

// the setup function runs once when power the board
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect
  }
  
  // Now set up two tasks to run independently.
   xTaskCreate(
    FirstTask
    ,  "FirstTask"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL //Parameters passed to the task function
    ,  1  // Priority, with 2 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  &FirstTask_Handler );//Task handle

   xTaskCreate(
    SecondTask
    ,  "SecondTask"
    ,  128  // Stack size
    ,  NULL //Parameters passed to the task function
    ,  2  // Priority
    ,  &SecondTask_Handler );  //Task handle
}
    

void loop()
{
  // Empty. Things are done in Tasks.
}


void SecondTask(void* pvParameters)
{
  (void) pvParameters;
   for (;;) // A Task shall never return or exit.
   {
    while(Serial.available()>0){
      switch(Serial.read()){
        case 's':
          vTaskSuspend(FirstTask_Handler); 
          Serial.println("Suspend!");
          break;
        case 'r':
          vTaskResume(FirstTask_Handler);
          Serial.println("Resume!");
          break;
      }
      vTaskDelay(1);
    }
   }
}

void FirstTask(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  for (;;) // A Task shall never return or exit.
  {
    //Serial.println(11);
    Serial.print("firstTask \n");  
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    Serial.print("SecondTask");   
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}
