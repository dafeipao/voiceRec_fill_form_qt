# iFlytek语音识别API + QT表单填写

该项目是一个QT程序，使用iFlytek的语音识别功能帮助您注册表单数据，特别是用于注册表单中的姓名和分数。程序集成功能包括：用户可以动态申请表单大小，设置表头名称，设置列模式（姓名、成绩、单个数字），自由编辑表单内容，支持自由复制和删除。

该项目使用iFlytek语音听写（流式版本）API进行语音识别，使用本地处理策略分离关键字，查询姓名集，并识别数字。

## 本地文本识别策略

*拼音转换（Python脚本调用）*
1. `getPinyinFromPython`： 使用Python脚本`pinyin_stcript.exe`将输入的中文文本转换为拼音。
2. 初始化Python环境并设置路径。
3. 调用Python函数`convert_to-pinyin`进行拼音转换。
4. 返回转换后的拼音。

*文本处理*
1. `processText`：根据给定的模式（姓名、成绩、1个数字）处理文本。处理文本并使用拼音修正方法`autoCorrectByPinyin`返回结果。
2. 模式处理：
   - 姓名：调用`processName`处理姓名，找到最佳匹配的姓名。
   - 成绩：调用`processGrade`处理成绩，将拼音转换为成绩（数字）。
   - 单个数字：调用`processSingleDigit`处理单个数字。

*拼音修正*
1. `autoCorrectByPinyin`：根据拼音处理文本并返回修正结果。
2. 先清理文本中的标点符号。
3. 根据模式（姓名、成绩等）判断如何进行拼音转换，并根据模式匹配姓名或数字。
4. 对于姓名，使用拼音对照表和候选姓名列表进行滑动窗口匹配，计算分数，选择最佳匹配。
5. 对于成绩和数字，通过拼音映射表将拼音转换为数字。

*姓名候选评估*
1. `evaluateNameCandidates`：将输入的拼音与加载的姓名列表中的拼音进行比较，使用滑动窗口算法找到最匹配的姓名，并根据相似度分数进行排序。
2. 根据拼音的全拼和首字母匹配得分。
   匹配规则：
   - (1) 首字母匹配。
   - (2) 全拼匹配。
   - (3) 位置惩罚。
   - (4) 长度差惩罚。
3. 计算姓名匹配得分，并根据匹配程度和位置进行排序。

*拼音缓存*
1. `getCachedPinyin`：使用缓存机制避免重复调用Python脚本进行拼音转换。
2. 如果文本的拼音已经被缓存，直接返回缓存结果；否则，调用`getPinyinFromPython`进行转换并缓存结果。

## 使用iFlytek语音听写（流式版本）API

**核心功能：**
1. 支持基于帧的音频数据传输（首帧/中间帧/末帧）
2. L16格式PCM音频，采样率16000Hz（原始编码）
3. 自动处理实时识别结果和最终结果

**安全认证**
1. 动态生成带HMAC-SHA256签名的认证URL
2. 认证机制包括API Key/Screen和AppID

**协议交互**
1. 使用WebSocket协议（WSS）
2. 按照iFlytek接口规范构建JSON请求体

**接口认证**

*生成认证URL（`generateAuthURL`）：*
步骤：
1. 获取当前时间（UTC），并将其格式化为特定的字符串格式（ddd, dd MMM yyyy HH: mm: ss' GMT '）。
2. 构造签名原文，包括：
    - 主机（目标服务器的主机名）
    - 日期（当前日期）
    - 请求的HTTP方法和路径（如：GET /v2/iat HTTP/1.1）。

3. 使用HMAC-SHA256算法计算签名。在计算过程中，`m_apiSecret`是密钥，`signtureOrigin`是数据源。
4. 将计算出的签名和其他必要信息（api_key、算法、头信息）合并到授权字符串中。
5. 编码授权、日期和主机等信息，并构造认证请求URL，形式为URL查询字符串。
**返回值**：构造好的认证URL。

*HMAC-SHA256签名生成（`hmacSha256`）：*
步骤：
1. 使用`QMessageInstanceCode`类计算HMAC-SHA256签名。
2. 设置密钥和待签名数据。
3. 返回Base64编码的签名结果。

*认证信息：*
认证信息包括以下内容：
1. Api_key：API密钥。
2. 算法：签名算法（hmac-sha256）。
3. 头信息：用于签名计算的HTTP头（包括主机、日期、请求行）。
4. 签名：使用HMAC-SHA256算法计算出的签名。

这些信息通过URL查询字符串添加到请求中，以验证身份和请求的合法性。

**如何使用**

1. 进入[*iFlytek控制台网站*](https://console.xfyun.cn/services/iat)注册并获取您的**APPID**、**APISecret**和**APIKey**。
2. 打开`voice.exe`，点击左上角的“设置”和“配置”，弹出配置界面。
3. 填写您的**APPID**、**APISecret**、**APIKey**和**nameList路径**（txt文件，每行一个姓名）。点击“保存”。
4. 选择每列的表头和模式，根据您的需要填写，或者指定名称。
5. 然后，您可以长按并按住“按住说话”按钮，开始讲话，语音会自动被识别。
6. 在右侧，您可以动态设置要填写的位置和范围。您还可以右键单击表格，设置当前填写位置。
7. 所有表格已设置为可以复制。选择区域后，按“ctrl+C”或右键选择“复制”。


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
