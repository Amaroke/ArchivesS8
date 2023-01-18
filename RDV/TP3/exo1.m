function main
N = 500;
t = linspace(0, 16*pi, 500);
v = cos(2*t);

im1 = ones(N, 1).*t;
im1show = (im1 - min(min(im1)))/(max(max(im1) - min(min(im1))));
%figure; imshow(im1show);

im2 = im1';
im2show = (im2 - min(min(im2)))/(max(max(im2) - min(min(im2))));
%figure; imshow(im2show);

im3 = cos(im1 - im2);
im3show = (im3 - min(min(im3)))/(max(max(im3) - min(min(im3))));
%figure; imshow(im3show);

t = linspace(0, 2*pi, 500);
v = cos(2*t);
im4 = ones(N, 1).*v;
im4show = (im4 - min(min(im4)))/(max(max(im4) - min(min(im4))));
%figure; imshow(im4show);

im5 = im3 .* im4;
im5show = (im5 - min(min(im5)))/(max(max(im5) - min(min(im5))));
figure; imshow(im5show);
end

