im = imread('animal_tres_mechant.jpg');
%figure;imshow(im);
im = double(rgb2gray(im))/255;
%figure;imshow(im);
cercle = circle(size(im,2)/2,size(im,1)/2, 10);

bw = poly2mask(cercle.XData, cercle.YData, size(im,1), size(im,2));

vf = fft(bw);
test = im .* 1-vf;
vf = fftshift(test);
imshow(vf);

%SPOILER ça marche pas et j'ai la flemme d'y réfléchir plus.