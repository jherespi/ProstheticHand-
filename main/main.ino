#include <Arduino.h>

/*Libraries*/
// Tensorflow model converted to C++
#include "model_data.h"
// functions
#include "functions.h"

#include "servoControl.h"

// Tensorflow custom library for ESP32
#include <TensorFlowLite_ESP32.h>

// experimental versions of regular Tensorflow headers
#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 70 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

int this_predict = -1;
int last_predict = -1;
int rotateCounts = 1;
int closeCounts = 1;
int id_servo[] = { 8, 10, 4, 12, 0}; 
/*Functions*/

void setup() {

  Serial.begin(115200);
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(prosthetic_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
      "Model provided is schema version %d not equal "
      "to supported version %d.",
      model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }
  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::ops::micro::AllOpsResolver resolver;


  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Keep track of how many inferences we have performed.
  inference_count = 0;
  setup3Sensor();
  setupServoDriver();
}

void loop() {

  input->data.f = prelim_collection();

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed on x_val");
    return;
  }

  Serial.println(" ");
  Serial.println("************************************************");
  Serial.println("----------------------------------");
  Serial.print("MODEL OUTPUT: Rest(Abrir):");
  Serial.print(output->data.f[0]);
  Serial.print(", Front(Anterior): ");
  Serial.print(output->data.f[1]);
  Serial.print(", Back(Posterior): ");
  Serial.println(output->data.f[2]);


  for (int i = 0; i < 3; i++) {
    if (output->data.f[i] >= 0.8)
      this_predict = i;
  }
  // If the new prediction is the same as the previous one servos should not move.
  if (this_predict != last_predict) {


    switch (this_predict) {
      case (0):

        Serial.println("OPENING HAND");
        openHand(id_servo);

        break;
      case (1):
        switch (rotateCounts) {
          case (1):
            rotate_cw(15);
            Serial.println("Rotate HAND");
            rotateCounts += 1;
            break;
          case (2):
            rotate_ccw(15);
            Serial.println("Rotate HAND");
            rotateCounts += 1;
            break;
          case (3):
            rotate_ccw(15);
            Serial.println("Rotate HAND");
            rotateCounts += 1;
            break;
          case (4):
            rotate_cw(15);
            rotateCounts = 1;
            Serial.println("Rotate HAND");
            break;
        }
        break;
      case (2):
        Serial.println("Close HAND");
        closeHand(id_servo);
        break;
    }
  }
  last_predict = this_predict;
  inference_count += 1;
  if (inference_count >= 5000)
    inference_count = 0;
}
