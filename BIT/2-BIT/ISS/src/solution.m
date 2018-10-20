% Subor: solution.m
% Popis: Riesenie projektu ISS 2014/2015
% Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
% Datum: 2015-01-01

img = imread('xklima22.bmp');

% zaostrenie obrazu
step1_filter = [-0.5 -0.5 -0.5; -0.5 5 -0.5; -0.5 -0.5 -0.5];
img_step1 = imfilter(img, step1_filter);
imwrite(img_step1, 'step1.bmp');

% otocenie obrazu
img_step2 = fliplr(img_step1);
imwrite(img_step2, 'step2.bmp');

% medianovy filter
img_step3 = medfilt2(img_step2, [5 5]);
imwrite(img_step3, 'step3.bmp');

% rozmazavanie obrazu
step4_filter = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1] / 49;
img_step4 = imfilter(img_step3, step4_filter);
imwrite(img_step4, 'step4.bmp');

% chyba v obraze
eimg1 = double(img);
eimg2 = fliplr(img_step4);
eimg2 = double(eimg2);
noise = 0;

for (x = 1:512)
  for (y = 1:512)
    noise = noise + abs(eimg1(x,y) - eimg2(x,y));
  end;
end;

noise = noise / 512 / 512

% roztiahnutie histogramu
mean_no_hist = mean2(img_step4)
std_no_hist = std2(img_step4)

img_dbl = im2double(img_step4);
minimum = min(img_dbl);
l_in = min(minimum);
maximum = max(img_dbl);
h_in = max(maximum);
l_out = 0.0;
h_out = 1.0;

img_step5 = imadjust(img_step4, [l_in h_in], [l_out h_out]);
imwrite(img_step5, 'step5.bmp');

mean_hist = mean2(img_step5)
std_hist = std2(img_step5)

% kvantizacia obrazu
x = size(img_step5);
N = 2;
a = 0;
b = 255;
img_dbl = double(img_step5);
img_step6 = zeros(x(1),x(2));
for k=1:x(1)
  for l=1:x(2)
    img_step6(k,l) = round(((2^N)-1)*(img_dbl(k,l)-a)/(b-a))*(b-a)/((2^N)-1) + a;
  end
end

img_step6 = uint8(img_step6);
imwrite(img_step6, 'step6.bmp');

% END solution.m
