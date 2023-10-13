# ComputerVision
how computers can understand digital images or videos just like automate tasks that human vision system can do [[blog]](https://hu-coding.tistory.com/category/Computer%20Vision)

* Preprocessing : 이미지 크기 조정, 색상 보정, 노이즈 제거 등
* Edge Extraction/Line Detection
* Image/Video Segmentation : 이미지를 구성하는 픽셀들을 객체나 영역으로 분할
* Object Detection/Tracking : 특정 객체를 식별하고 추적
* Image Transformation

# Background
## History
<img src='https://global-uploads.webflow.com/6434d6a6071644318551fd72/64cc7f679407b4fff427ed6e_03%20what%20is%20computer%20vision_datahunt.webp'>

* 1960s : Perceptron 개념을 기반으로 Image Pattern Recognize 연구가 시작됨. 픽셀 수준의 처리와 패턴 인식에 초점이 맞춰짐 [[survey]](https://books.googleusercontent.com/books/content?req=AKW5QafDdn36GTbbsElGv88bx2_x63DgAIxC7rP_90r72jmzggw9jXRm7yNIbfJWs-XpKUTaaKIYGfjhB1H4jwlsq5R8VXxY6sjTZEz_CX_E71wt-PPLnBL8u2_YdLa8FjV4GoRExUdOmj4-xTi1rrqRpZtDx6llsr7nyy2hPnAaOe3baCBGAzNQaP6ZTO5EO5a1_19fztVXLeWZ3nAqy13Ibwm8BnO5FtRjHD1f9wo8xMJJCReB5bSDVwzdT7-RI1hFN_SLKVYeBRNLujKSRPX5a6BZOPS47fZVv3pQ9IgQYw05fxmnZAQ)
* 1970~1980s : edge detection, histogram equalization과 같은 이미지 처리 알고리즘의 이론적 토대와 기술적 발전이 이루어짐
* 1990s : 복잡하고 정교한 알고리즘과 방법론이 도입됨. Object Detection/Tracking, Segmentation 기술이 등장함
* 2000s : 딥러닝과 신경망 알고리즘의 등장으로 모델의 성능이 획기적으로 향상되며, 얼굴인식 및 객체 분류 등 다양한 응용 사례가 등장함
실시간 처리 능력을 강조한 Model-centric 개발이 우선되었으나, 최근에는 실시간성뿐만 아니라 학습 데이터 품질 향상을 통한 정확성 증진도 활발히 연구되고 있음. 또한, 기존의 CNN[[video]](https://www.youtube.com/watch?v=ixF5WNpTzCA)뿐만 아니라, 전체 시퀀스의 정보를 한번에 처리할 수 있는 "Attention Mechanism"이 도입된 Transformers가 주목받고 있음.

## Basics of Digital Image/Video
* Pixel : smallest unit of Image (1~4 values)
<img width="564" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/918bcc11-6319-4bef-bbc0-1c8eede4d0a4">

* Intensity Level : 각각의 <b>pixel<B>이 가질 수 있는 표현의 개수. 2의 지수승으로 존재
  - Normally, 256($2^8$)
  - Intensity가 클수록 정교하게 표현할 수 있음
  <img width="294" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/931569e8-1544-441f-8050-cb0e8d314cda">
  
* Pixel Resolution : 해상도. <b>image<b>가 가진 픽셀의 개수
  - Total Number of bits to store a digital image = the number of rows(height) * the number of columns(width) * the number of bits for one pixel(intensity level)

* FPS : the number of images(frames) of a <b>video<b> per second
  - the interval is normally 33ms (*30fps=1000ms=1s)
  - FPS가 클수록, interval이 낮을 수록 자연스러움

# Intensity Transformation
Way to enhance the image <br>
Using Mapping function, ~~
* Negative Transformation
* Log Transformation : enhance contrast of dark region
* Gamma Transformation : gamma < 1

thresholding

# Filtering
## Histogram Filtering
Depending on the number of bins, the result would change dramatically. <br>
Normalized Histogram (PDF) - can calculate the probability of pixel value
## Histogram Equlization
low contrast image vs high contrast image <br>
intensity transformation + histogram filtering? <br>
In the Moon.png, 
## Spatial Filtering
1 Define the kernel size (3,3), (5,5), ...
2 Scan with masking
* Average Filtering : blur, smoothing, nosie ?
* Gausian Filtering
* Second Derivative - Laplacia? : sharpening
* Unsharp Masking : sharpening
* Median Filtering : remove noise without blurry but need more computation

# Color Image Enhancement
How to enhance color image
## Color Space
* RGB
* HSI
* YUV
accromatic color 무채색?
## Color Conversion
## Color Slicing
* Hue Channel
* X Channel?
## White balancing
## Gray World Assumption

# Edge/Line Detection
Edge pixel : a lot of intensity difference
* Sobel Mask
* Canny : 1 blur 2 sobel to calculate gradient 3 non maxima suppresion 4 double thresholding 5 connectivity analysis

# Image Segmentation
1 back / forward
2 thresholding : how to defined threshoding is very important job
- gobal : Basing, Ostant? : performance measurement : within-class variance / between-class variance
- local : average adjacent pixel

# Video Segmentation (Background Subtraction)
1 background
- GMM : p(B|A) and p(A|B) => p(X|background) and p(Background|X) : the form of ML
2 How to subtract
