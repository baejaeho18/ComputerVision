# 0. ComputerVision
how computers can understand digital images or videos just like automate tasks that human vision system can do [[blog]](https://hu-coding.tistory.com/category/Computer%20Vision)

* Preprocessing : 이미지 크기 조정, 색상 보정, 노이즈 제거 등
* Edge Extraction/Line Detection
* Image/Video Segmentation : 이미지를 구성하는 픽셀들을 객체나 영역으로 분할
* Object Detection/Tracking : 특정 객체를 식별하고 추적
* Image Transformation

# 1. Background
## 1.1 History
<img src='https://global-uploads.webflow.com/6434d6a6071644318551fd72/64cc7f679407b4fff427ed6e_03%20what%20is%20computer%20vision_datahunt.webp'>

* 1960s : Perceptron 개념을 기반으로 Image Pattern Recognize 연구가 시작됨. 픽셀 수준의 처리와 패턴 인식에 초점이 맞춰짐 [[survey]](https://books.googleusercontent.com/books/content?req=AKW5QafDdn36GTbbsElGv88bx2_x63DgAIxC7rP_90r72jmzggw9jXRm7yNIbfJWs-XpKUTaaKIYGfjhB1H4jwlsq5R8VXxY6sjTZEz_CX_E71wt-PPLnBL8u2_YdLa8FjV4GoRExUdOmj4-xTi1rrqRpZtDx6llsr7nyy2hPnAaOe3baCBGAzNQaP6ZTO5EO5a1_19fztVXLeWZ3nAqy13Ibwm8BnO5FtRjHD1f9wo8xMJJCReB5bSDVwzdT7-RI1hFN_SLKVYeBRNLujKSRPX5a6BZOPS47fZVv3pQ9IgQYw05fxmnZAQ)
* 1970~1980s : edge detection, histogram equalization과 같은 이미지 처리 알고리즘의 이론적 토대와 기술적 발전이 이루어짐
* 1990s : 복잡하고 정교한 알고리즘과 방법론이 도입됨. Object Detection/Tracking, Segmentation 기술이 등장함
* 2000s : 딥러닝과 신경망 알고리즘의 등장으로 모델의 성능이 획기적으로 향상되며, 얼굴인식 및 객체 분류 등 다양한 응용 사례가 등장함
실시간 처리 능력을 강조한 Model-centric 개발이 우선되었으나, 최근에는 실시간성뿐만 아니라 학습 데이터 품질 향상을 통한 정확성 증진도 활발히 연구되고 있음. 또한, 기존의 CNN[[video]](https://www.youtube.com/watch?v=ixF5WNpTzCA)뿐만 아니라, 전체 시퀀스의 정보를 한번에 처리할 수 있는 "Attention Mechanism"이 도입된 Transformers가 주목받고 있음.

## 1.2 Basics of Digital Image/Video
* Pixel : smallest unit of Image (1~4 values)
<img width="200" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/918bcc11-6319-4bef-bbc0-1c8eede4d0a4">

* Intensity Level : 각각의 pixel이 가질 수 있는 표현의 개수. 2의 지수승으로 존재
  - Normally, 256( $2^8$ )
  - Intensity가 클수록 정교하게 표현할 수 있음
<img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/931569e8-1544-441f-8050-cb0e8d314cda">
  
* Pixel Resolution : 해상도. image가 가진 픽셀의 개수
  - Total Number of bits to store a digital image = the number of rows(height) * the number of columns(width) * the number of bits for one pixel(intensity level)
  - 640x360이 기본 : VGA(1,1.5) - HD(2,2) - FHD(3,3) - QHD(4,4) - UHD(6,6)

* FPS : the number of images(frames) of a video per second
  - the interval is normally 33ms (*30fps=1000ms=1s)
  - FPS가 클수록, interval이 낮을 수록 자연스러움

# 2. Intensity Transformation
<img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/3cf2a450-a6f0-4dad-81a0-74570b9b5203"> <br>
Way to enhance the image, using following functions:

## 2.1 Mapping function
* Negative Transformation :  $input+output = max intensity$
* Log Transformation : $output = c*log(input+1) $
  - enhance contrast of dark region
<img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/e8e399bc-4e6d-43a7-9933-f62b18a54c75">

* Gamma(power-law) Correction : $output = c*input^r$
  - gamma < 1 : enhance the contrast of dark region
  - gamma = 1 : identity
  - gamma > 1 : enhance the contrast of bright region
<img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/01123ec0-1594-4f3f-932e-0968527b34ea">

* Piecewise-linear Transformation : more complex
  - Thresholding is also possible
<img width="150" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/058eafc7-f6d3-46ba-b9c8-effee883f3a9">

## 2.2 Filtering
### 2.2.1 Spatial Filtering
Spatial filters : spatial masks, kernels, templates, windows..

1) Define the kernel size (3,3), (5,5), ...
2) Scan with masking
* Average Filtering : replace the value of the pixel by the average of the intensity levels in the neighborhood
  - reduce random noises
  - blur image
* Gausian Filtering : set weight to neighborhood
  - Discretiezed Gaussian Kernel
  - Floating-point Gaussian kernel
 <img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/f55c5d8a-cc1d-4e33-98d1-4acff194e97c">

* sharpening : highlight transitions in intensity
  - Second Derivative : f'' = f(x+1)+f(x-1)-2f(x) | Sum = Laplacian
    
    <img width="200" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/67714d10-96ce-41ba-b695-6534f32296c5">

  - Unsharp Masking
    
    <img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/09532796-2b7f-4daf-9e57-3d57f08b47fd">

* Median Filtering : replace the value with the median value of a mask (3x3 -> 5th largest)
  - remove noise without blurry but need more computation
  - effective at impulse(sale-and-pepper noise)

### 2.2.2 Histogram Equalizationi
Depending on the number of bins, contrast of an image adujsted.
 - Contranst : The difference in brig;htness or color that makes an object distinguishable

CDF(Cumulative Distribution Function) : calculate the probability of pixel value
<img width="1170" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/853f8077-6b5b-4cec-8be9-7751c7f0ff1f">

# 3. Color Image Enhancement
How to enhance color image
## 3.1 Color Model
* RGB
  
  <img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/0e74e179-2f7b-4534-ab30-f561a7544045">

* HSI : Hue(색상:0~360°), Saturation(Clear), Intensity(Brightness)

  <img width="500" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/66310254-c65c-4977-ba86-c0f9d99277dd">

* YUV(YCbCr) : Y(Luma:Brightness), Chroma Blue(Blue - Y), Chroma Red(Red - Y)

  <img width="300" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/ca862625-c5bd-4578-a93d-3b8c39f043df">

- Grayscale image : lightness is the only parameter of a pixel that can vary
- Achromatic Color : gray, black, white

## 3.2 Color Processing
Intsity transformation, histogram equalization, spatial filtering are applied on the intensity channel only!
* Color Conversion : CV_BGR2HSV, CV_BGR2GRAY

It means, converting color space into HSI or YUV from RGB is useful. 
* Pseudo Coloring : gray image to color image
* Color Slicing : Find the pixels in the range of the desired color in the Hue-channel. Set all the other pixels to 0 in the Saturation-channel (grayscale image)
* White balancing : global adjustment of the intensities of the colors
  - Gray World Assumption : the average of all the colors is a neutral gray : $result = original*(128/average)$

# 4. Edge/Line Detection
Edge : a lot of intensity difference / Image smoothing for noise reduction should be performed(like, mediean/average filtering)
* Sobel Mask

  <img width="200" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/9c28f1db-d32c-40fe-9548-f12d59c4e4bb"> <br>

  <img width="200" alt="image" src="https://github.com/baejaeho18/ComputerVision/assets/37645490/6e60cece-c8d0-4edf-911f-96bfc30e1088">

* Canny
  - smooothing : remove noise
  - sobel to calculate gradient : angle and derviative
  - non maxima suppresion : choose only the maximun
  - double thresholding & connectivity analysis : determine whether is edge or not

Line
* Hough Transform ?
  - obtain a binary edge image
  - specify subdivision in the plane
  - examine the ocunts of the accumulator cells for hight pixel concentrations

same as circle detection


# 5. Image Segmentation
Process of partitioning a digital image into multiple region. 

1 back / forward ?
2 thresholding : how to defined proper threshoding (between background and object) is important
* gobal : Basing, Ostant? : performance measurement : within-class variance / between-class variance
  - Basic : repeat T=(m1+m2)/2 until the change is small enough
  - Otsu : compute between-class variance which is maximized
* local(adaptive) : set a threshold depending on the intensity distribution of adjacent pixel
3 GrabCut ?

# 6. Video Segmentation (Background Subtraction)
1 background
- GMM : p(B|A) and p(A|B) => p(X|background) and p(Background|X) : the form of ML
2 How to subtract
- 

## Morphological Operation
- Erosion -> Opening : breaks narrow isthmuses and eliminates small island and sharp peaks
- Dilation -> Closing : fueses narrow breaks and long thin gulfs and eliminates small holes

# 9. Image Feature Matching
Image Feature : piece of information that is relevant for solving the computational task such as specific structures(points, edges, objects) <br>
Good feature = inexpensive and memory efficient
* ORB : oFast detector + r-BRIEF descriptor
  - Fast : Determines the corner by having more than N consecutive pixels whose intestities are higher or lowe

  ![image](https://github.com/baejaeho18/ComputerVision/assets/37645490/0fc95bff-7e8a-4a6f-84ab-2c8de55298cc)

  - BRIEF : A bit string descriptor of an image patch constructed from a set of binary intensity tests
* NNDR(Nearest neighbor distance ratio) $= frac{distance to best match}{distance to second best match}$

## CNN

![image](https://github.com/baejaeho18/ComputerVision/assets/37645490/9cba7d9c-66f9-426f-a7d8-6b53fc66a68c)
- Convolution
- Relu
- Pooling
![image](https://github.com/baejaeho18/ComputerVision/assets/37645490/574ab8be-0075-452a-a67a-ba637e33159e)

# 10. Detection Tracking


# 11. Object Detection using Deep Learning

# 12. Projective Transformation

# 13. Understanding of a Camera

# 14. Image Compression

Feature => SIFT, IRB, corners
- extraction : position of features		(FAST)
- description : ready for feature matching	(BRIEF) >:0 <=:1 -> binary string?
NNDR : best match > second best match?


Detection & Tracking
- Face Detecter : Harr-like feature
traking=find good harr-like feature
boosting : sequetially do it
Stroing learneer : use samll amount of weak learner -> 




