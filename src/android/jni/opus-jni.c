/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <opus.h>
#include <stdio.h>

OpusEncoder *encoder;
OpusDecoder *decoder;
int bitrate = 16000;

#define EXIT_FAILURE -1
#define SAMPLE_RATE 16000
#define CHANNELS 1
#define APPLICATION OPUS_APPLICATION_AUDIO
#define FRAME_SIZE 960
#define MAX_PACKET_SIZE (3*960)

jint Java_com_exelerus_cordova_audioinputcapture_OpusJni_opusOpen(JNIEnv *env, jobject thiz, jint bitrate) {
  int err;

  //Create encoder
  encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, APPLICATION, &err);
  if (err<0)
  {
     __android_log_print(ANDROID_LOG_INFO, "Opus", "failed to create an encoder: %s", opus_strerror(err));
     return EXIT_FAILURE;
  }

  err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitrate));
  if (err<0)
  {
     __android_log_print(ANDROID_LOG_INFO, "Opus", "failed to set bitrate: %s", opus_strerror(err));
     return EXIT_FAILURE;
  }

  //Create decoder
  decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &err);
   if (err<0)
   {
      __android_log_print(ANDROID_LOG_INFO, "Opus", "failed to create decoder: %s", opus_strerror(err));
      return EXIT_FAILURE;
   }

  return err;
}

void char2short(signed char* pchar, opus_int16 *pshort)
{
  *pshort = (pchar[0] << 8) | pchar[1];
}

jint Java_com_exelerus_cordova_audioinputcapture_OpusJni_opusDecode(JNIEnv *env, jobject thiz,
   jshortArray jencoded, jint jencodedOffset, jint jencodedLength, jshortArray joutputBuffer, jint jpcmOffset, jint jframeSize) {

   opus_int16* outputBuffer = (opus_int16 *) malloc(FRAME_SIZE * sizeof(opus_int16));
   unsigned char* inputBuffer = (unsigned char*) (*env)->GetShortArrayElements(env, jencoded, NULL);

   int frame_size = opus_decode(decoder,
     inputBuffer,
     jencodedLength,
     outputBuffer,
     FRAME_SIZE,
     0);

   if (frame_size<0)
   {
      __android_log_print(ANDROID_LOG_INFO, "Opus", "decoder failed: %s", opus_strerror(frame_size));
   } else {
     (*env)->SetShortArrayRegion(env, joutputBuffer, 0, frame_size, (jshort *)outputBuffer);
   }

   return frame_size;
}

jint Java_com_exelerus_cordova_audioinputcapture_OpusJni_opusEncode(JNIEnv *env, jobject thiz,
   jshortArray jinputBuffer, jint jpcmOffset, jint jpcmLength, jshortArray jencoded, jint jencodedOffset) {

    unsigned char *outpacket = (unsigned char *) malloc(MAX_PACKET_SIZE * sizeof(unsigned char));

    opus_int16* inputBuffer = (opus_int16*) (*env)->GetShortArrayElements(env, jinputBuffer, NULL);

    int len = opus_encode(encoder,
                          inputBuffer, //
                          jpcmLength,
                          outpacket, //
                          MAX_PACKET_SIZE);

     if (len < 0)
     {
       __android_log_print(ANDROID_LOG_INFO, "Opus", "Opus encoding -> error: = %s", opus_strerror(len));
     } else {
       (*env)->SetShortArrayRegion(env, jencoded, 0, len, (jshort *)outpacket);
     }
    return len;
}

jint Java_com_exelerus_cordova_audioinputcapture_OpusJni_opusClose(JNIEnv *env, jobject thiz) {
  opus_encoder_destroy(encoder);
  opus_decoder_destroy(decoder);
  return 0;
}
