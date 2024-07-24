/* Edge Impulse ingestion SDK
 * Copyright (c) 2022 EdgeImpulse Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// If your target is limited in memory remove this macro to save 10K RAM
#define EIDSP_QUANTIZE_FILTERBANK   0

/**
 * Define the number of slices per model window. E.g. a model window of 1000 ms
 * with slices per model window set to 4. Results in a slice size of 250 ms.
 * For more info: https://docs.edgeimpulse.com/docs/continuous-audio-sampling
 */
#define EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW 4

/*
 ** NOTE: If you run into TFLite arena allocation issue.
 **
 ** This may be due to may dynamic memory fragmentation.
 ** Try defining "-DEI_CLASSIFIER_ALLOCATION_STATIC" in boards.local.txt (create
 ** if it doesn't exist) and copy this file to
 ** `<ARDUINO_CORE_INSTALL_PATH>/arduino/hardware/<mbed_core>/<core_version>/`.
 **
 ** See
 ** (https://support.arduino.cc/hc/en-us/articles/360012076960-Where-are-the-installed-cores-located-)
 ** to find where Arduino installs cores on your machine.
 **
 ** If the problem persists then there's not enough memory for this model and application.
 */

/* Includes ---------------------------------------------------------------- */
#include <PDM.h>
#include <color_commandKeyword_inferencing.h>

/** Audio buffers, pointers and selectors */
typedef struct {
    signed short *buffers[2];
    unsigned char buf_select;
    unsigned char buf_ready;
    unsigned int buf_count;
    unsigned int n_samples;
} inference_t;

static inference_t inference;
static bool record_ready = false;
static signed short *sampleBuffer;
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
static int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);

//Global Variable Flags for Commands:
int color = 0, color2 = 0, noVoiceCount = 0;
bool wakeUp, listenMode;
bool ledKey, ledON, andKey, blinkKey, cancelKey, fastKey, flashKey, plusKey, quickKey, slowKey, toggleKey;

//Plus Specific Commands:
int colorP = 0, color2P = 0;
bool ledKeyP, ledOnP, andKeyP, blinkKeyP, fastKeyP, flashKeyP, quickKeyP, slowKeyP, toggleKeyP;

//LED Pins
const int ledRedPin = 22;
const int ledGreenPin = 24;
const int ledBluePin = 23;


/**
 * @brief      Arduino setup function
 */
void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    // comment out the below line to cancel the wait for USB connection (needed for native USB)
    while (!Serial);
    Serial.println("Edge Impulse Inferencing Demo");

    // summary of inferencing settings (from model_metadata.h)
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) /
                                            sizeof(ei_classifier_inferencing_categories[0]));

    run_classifier_init();
    if (microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false) {
        ei_printf("ERR: Could not allocate audio buffer (size %d), this could be due to the window length of your model\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
        return;
    }
    //Set built-in LED as outpu
    pinMode(LED_BUILTIN, OUTPUT);
    //Set LED as outputs
    pinMode(ledRedPin, OUTPUT);
    pinMode(ledGreenPin, OUTPUT);
    pinMode(ledBluePin, OUTPUT);
    //LEDs OFF by Default
    analogWrite(ledRedPin, 255);
    analogWrite(ledGreenPin, 255);
    analogWrite(ledBluePin, 255);
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */
void loop()
{
    bool m = microphone_inference_record();
    if (!m) {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }

    if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)) {
        /*
          CODE AT THIS POINT MODIFIED FOR FUNCTIONALITY
          -> LOOKS FOR THE KEYWORDS AND TURNS ON LED IF HEARD
        */
        int predict = 0;
        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) 
        {
            //ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
            if(result.classification[ix].value > result.classification[predict].value)
            {
              predict = ix;
            }
            if(result.classification[predict].value > 0.60)
            {
              predict;
            }
            else
            {
              predict = 30;
            }

        }
        if(predict != 30 && predict != 16 && predict != 17 && predict != 22)
        {
          noVoiceCount = 0;
          ei_printf("Guess: %s \n", result.classification[predict].label);
          if(predict == 8)
          {
            digitalWrite(LED_BUILTIN, HIGH);
            wakeUp = true;
          }
        }
        else
        {
          noVoiceCount++;
        }
        if(noVoiceCount > 20)
        {
          wakeUp =  false;
          digitalWrite(LED_BUILTIN, LOW);
        }
        else if(wakeUp == true)
        {
          //Start Chip Logic
          toggleFlags(predict);
          if(cancelKey == true) resetFlags();
        }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

        print_results = 0;
    }
}
void toggleFlags(int predict)
{
  switch (predict) 
  {
    case 0: //Blue (color = 1)
      (plusKey) ? ((andKeyP) ? color2P = 1 :  colorP = 1) : ((andKey) ? color2 = 1 : color = 1);
    break;
    case 1: //Cyan (color = 3)
      (plusKey) ? ((andKeyP) ? color2P = 3 :  colorP = 3) : ((andKey) ? color2 = 3 : color = 3);
    break;
    case 2: //Green (color = 2)
      (plusKey) ? ((andKeyP) ? color2P = 2 :  colorP = 2) : ((andKey) ? color2 = 2 : color = 2);
    break;
    case 3: //LED (ledKey = true)
      switchLED();
    break;
    case 4: //Magenta (color = 5)
      (plusKey) ? ((andKeyP) ? color2P = 5 :  colorP = 5) : ((andKey) ? color2 = 5 : color = 5);
    break;
    case 5: //Off (color = 0)
      (plusKey) ? ((andKeyP) ? color2P = 0 :  colorP = 0) : ((andKey) ? color2 = 0 : color = 0);
    break;
    case 6: //On (ledON = true)
              
    break;
    case 7: //Red (color = 4)
      (plusKey) ? ((andKeyP) ? color2P = 4 :  colorP = 4) : ((andKey) ? color2 = 4 : color = 4);
    break;
    case 8: //Wake Up
      wakeUp = true;
    break;
    case 9: //White (color = 7)
      (plusKey) ? ((andKeyP) ? color2P = 7 :  colorP = 7) : ((andKey) ? color2 = 7 : color = 7);
    break;
    case 10: //Yellow (color = 6)
      (plusKey) ? ((andKeyP) ? color2P = 6 :  colorP = 6) : ((andKey) ? color2 = 6 : color = 6);
    break;
    case 11: //and (andKey = true)
      (plusKey) ? andKeyP = true : andKey = true;
    break;
    case 12: //blink (blinkKey = true)
      (plusKey) ? blinkKeyP = true : blinkKey = true;
    break;
    case 13: //cancel (cancelKey = true)
      cancelKey = true;
    break;
    case 14: //fast (fastKey = true)
      (plusKey) ? fastKeyP = true : fastKey = true;
    break;
    case 15: //flash (flashKey = true)
      (plusKey) ? flashKeyP = true : flashKey = true;
    break;
    case 16: //noise
             
    break;
    case 17: //noise2
              
    break;
    case 18: //plus (plusKey = true)
      plusKey = true;
    break;
    case 19: //quick (quickKey = true)
      (plusKey) ? quickKeyP = true : quickKey = true;
    break;
    case 20: //slow (slowKey = true)
      (plusKey) ? slowKeyP = true : slowKey = true;
    break;
    case 21: //toggle (toggleKey = true)
      (plusKey) ? toggleKeyP = true : toggleKey = true;
    break;
    case 22: //unknown
              
    break;
  }
}
void resetFlags()
{
  color = 5; color2 = 5; noVoiceCount = 0; 
  ledKey = false; ledON = false; andKey = false; blinkKey = false; cancelKey = false; 
  fastKey = false; flashKey = false; plusKey = false; quickKey = false; slowKey = false; toggleKey = false;
}
void switchLED()
{
  //Select Correct color variable
  int localColor = (plusKey) ? ((andKeyP) ? color2P : colorP) : ((andKey) ? color2 : color);

  //LED off (to change colors)
  analogWrite(ledRedPin, 255);
  analogWrite(ledGreenPin, 255);
  analogWrite(ledBluePin, 255);
  //Color Switch Protocall
  if(localColor == 0); //Off
  else if(localColor == 1) //Blue
  {
    analogWrite(ledBluePin, 0);
  }
  else if(localColor == 2) //Green
  {
    analogWrite(ledGreenPin, 0);
  }
  else if(localColor == 3) //Cyan
  {
    analogWrite(ledGreenPin, 0);
    analogWrite(ledBluePin, 0);
  }
  else if(localColor == 4) //Red
  {
    analogWrite(ledRedPin, 0);
  }
  else if(localColor == 5) //Magenta
  {
    analogWrite(ledRedPin, 0);
    analogWrite(ledBluePin, 0);
  }
  else if(localColor == 6) //Yellow
  {
    analogWrite(ledRedPin, 0);
    analogWrite(ledGreenPin, 0);
  }
  else if(localColor == 7) //White
  {
    analogWrite(ledRedPin, 0);
    analogWrite(ledGreenPin, 0);
    analogWrite(ledBluePin, 0);

  }
  else
  {
    ei_printf("No Read \n");
  }
}
int toggleListenMode()
{
  if(listenMode == false)
  {
    listenMode = true;

  }
  else if(listenMode == true)
  {
    listenMode = false;

  }
}

/**
 * @brief      PDM buffer full callback
 *             Get data and call audio thread callback
 */
static void pdm_data_ready_inference_callback(void)
{
    int bytesAvailable = PDM.available();

    // read into the sample buffer
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);

    if (record_ready == true) {
        for (int i = 0; i<bytesRead>> 1; i++) {
            inference.buffers[inference.buf_select][inference.buf_count++] = sampleBuffer[i];

            if (inference.buf_count >= inference.n_samples) {
                inference.buf_select ^= 1;
                inference.buf_count = 0;
                inference.buf_ready = 1;
            }
        }
    }
}

/**
 * @brief      Init inferencing struct and setup/start PDM
 *
 * @param[in]  n_samples  The n samples
 *
 * @return     { description_of_the_return_value }
 */
static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffers[0] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[0] == NULL) {
        return false;
    }

    inference.buffers[1] = (signed short *)malloc(n_samples * sizeof(signed short));

    if (inference.buffers[1] == NULL) {
        free(inference.buffers[0]);
        return false;
    }

    sampleBuffer = (signed short *)malloc((n_samples >> 1) * sizeof(signed short));

    if (sampleBuffer == NULL) {
        free(inference.buffers[0]);
        free(inference.buffers[1]);
        return false;
    }

    inference.buf_select = 0;
    inference.buf_count = 0;
    inference.n_samples = n_samples;
    inference.buf_ready = 0;

    // configure the data receive callback
    PDM.onReceive(&pdm_data_ready_inference_callback);

    PDM.setBufferSize((n_samples >> 1) * sizeof(int16_t));

    // initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate
    if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY)) {
        ei_printf("Failed to start PDM!");
    }

    // set the gain, defaults to 20
    PDM.setGain(127);

    record_ready = true;

    return true;
}

/**
 * @brief      Wait on new data
 *
 * @return     True when finished
 */
static bool microphone_inference_record(void)
{
    bool ret = true;

    if (inference.buf_ready == 1) {
        ei_printf(
            "Error sample buffer overrun. Decrease the number of slices per model window "
            "(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)\n");
        ret = false;
    }

    while (inference.buf_ready == 0) {
        delay(1);
    }

    inference.buf_ready = 0;

    return ret;
}

/**
 * Get raw audio signal data
 */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffers[inference.buf_select ^ 1][offset], out_ptr, length);

    return 0;
}

/**
 * @brief      Stop PDM and release buffers
 */
static void microphone_inference_end(void)
{
    PDM.end();
    free(inference.buffers[0]);
    free(inference.buffers[1]);
    free(sampleBuffer);
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor."
#endif
