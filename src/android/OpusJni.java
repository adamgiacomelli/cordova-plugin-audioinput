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
 */
package com.exelerus.cordova.audioinputcapture;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class OpusJni {

    public static native int opusOpen(int bitrate);
    public static native int opusEncode(short jpcm[], int jpcmOffset, int jpcmLength, short jencoded[], int jencodedOffset);
    public static native int opusDecode(short jencoded[], int jencodedOffset, int jencodedLength, short jpcm[], int jpcmOffset, int jframeSize);
    public static native int opusClose();

    static {
        System.loadLibrary("opus-jni");
    }
}
