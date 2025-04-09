# IFlytek Speech Recognition API + QT Form Filling

This project is a QT program that uses iFlytek's speech recognition to help you register form data, especially for registering form names and scores. The program integration functions include: users can dynamically apply for form size, set header names, set column modes (name, grade, single number), freely edit form content, and freely copy and delete.

This project uses iFlytek Voice Dictation (Streaming Version) API for speech recognition, with local processing using strategies to separate keywords, query name sets, and recognize numbers.

## Local Text Recognition Strategy

*Pinyin conversion (Python script call)*
1. getPinyinFromPython： Use the Python script pinyin_stcript.exe to convert the input Chinese text into pinyin.
2. Initialize the Python environment and set the path.
3. Call the Python function convert_to-pinyin to perform pinyin conversion.
4. Return the converted pinyin.

*Text processing*
1. processText： Process text according to the given pattern (name, grade, 1 digit). Process text and return results using the pinyin correction method autoCorrectByPinyin.
2. Mode processing:
Name: Call processName to process the name and find the best matching name.
Grade: Call processGrade to process grades and convert pinyin to grades (numbers).
One digit ": Call processSingleDigit to process a single digit.

*Pinyin correction*
1. autoCorrectByPinyin： Process the text based on pinyin and return the corrected result.
2. Clean up punctuation marks in the text first.
3. Determine how to perform pinyin conversion and match names or numbers based on the pattern (name, grade, etc.).
4. For names, use a pinyin comparison table and a list of candidate names to perform sliding window matching, calculate scores, and select the best match.
5. For grades and numbers, convert pinyin to numbers through a pinyin mapping table.

*Name candidate evaluation*
1. evaluateNameCandidates： Compare the input pinyin with the pinyin in the loaded name list, use the sliding window algorithm to find the most matching name, and sort it based on similarity score.
2. Score based on the full spelling and first letter matching of pinyin.
   Matching rule:
   (1)First letter matching.
   (2)Full Pinyin Matching.
   (3)Location penalty.
   (4)Punishment for length difference.
3. Calculate the score for name matching and sort by matching degree and position.
   
*Cache Pinyin*
1. getCachedPinyin： Use caching mechanism to avoid repeatedly calling Python scripts for Pinyin conversion.
2. If the pinyin of the text has already been cached, return the cached result directly; Otherwise, call getPinyinFromPython for conversion and cache the results.


## Use of iFlytek Voice Dictation (Streaming Version) API

**Core functions:**
1.Support frame based transmission of audio data (first frame/middle frame/last frame)
2.L16 format PCM audio with a sampling rate of 16000Hz (raw encoding)
3.Automatically process real-time recognition results and final results

**Security certification**
1.Dynamically generate authentication URLs with HMAC-SHA256 signature
2.Verification mechanism including API Key/Screen and AppID

**Protocol interaction**
1.Using WebSocket Protocol (WSS)
2.Building JSON Request Body Following iFlytek Interface Specification

**Interface authentication**

*Generate authentication URL (generateAuthURL):*
Steps:
1. Retrieve the current time (UTC) and format it into a specific string format (ddd, dd MMM yyyy HH: mm: ss' GMT ').
2. Construct the signed original text, which includes:

    Host (the host name of the target server)
    Date (current date)
    The HTTP method and path of the request (such as: GET /v2/iat HTTP/1.1).

3. Use the HMAC-SHA256 algorithm to calculate the signature. During the calculation process, m_apiSecret is the key and signtureOrigin is the data source.
4. Combine the calculated signature and other necessary information (api_key, algorithm, headers) into an authorization string.
5. Encode information such as authorization, date, and host and construct an authentication request URL in the form of a URL query string.
**Return value**: Constructed authentication URL.

*HMAC-SHA256 signature generation (hmacSha256):*
Steps:
1. Use the QMessageInstanceCode class for HMAC-SHA256 signature calculation.
2. Set the key and the data to be signed.
3. Return the Base64 encoded signature result.
   
*Authentication information:*
The authentication information includes the following:
1. Api_key: API key.
2. algorithm： Signature algorithm (hmac-sha256).
3. headers： The HTTP headers used in signature calculation (including host, date, request line).
4. signature： The signature calculated using the HMAC-SHA256 algorithm.
   
These pieces of information are added to the request through URL query strings to verify identity and the legitimacy of the request.

**How to use**

1. Enter [*the iFlytek Console website*](https://console.xfyun.cn/services/iat) to register and obtain your **APPID** , **APISecret** and **APIKey**.
2. Open voice.exe, click on "设置" and "配置" in the upper left corner, and the configuration interface will pop up.
3. Fill in your **APPID** , **APISecret** , **APIKey** and **nameList path**(txt,one name per line).Click "保存".
4. Select the header and mode of each column as the mode you need to fill in, or give it a name.
5. Then you can long press and hold "按住说话" while speaking, and your voice will be automatically recognized.
6. On the right side, you can dynamically set the position and range to be filled in. You can also use the right mouse button to select the table and set the current filling position.
7. All tables have been set up for copying. After selecting the area with the left mouse button, you can press "ctrl+C" or select "copy" with the right mouse button.