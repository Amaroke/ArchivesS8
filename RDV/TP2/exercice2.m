function exercice2()
    sRGB = imread("shadows.jpg");
    subplot(1,2,1),
    imshow(sRGB);
    [H,S,I] = getHSI(sRGB);
    [x,y] = ginput(1);
    h = H(int32(y), int32(x));
    H(find(abs(H-h)<pi/5)) = pi/2;
    HSI = cat(3, H, S, I);
    subplot(1,2,2),
    imshow(HSI2uRGB(HSI));
end

function [H,S,I] = getHSI(sRGB)
    uRGB = double(sRGB)/255;
    uR = uRGB(:, :, 1);
    uG = uRGB(:, :, 2);
    uB = uRGB(:, :, 3);
    x = uR - 0.5*(uG + uB);
    y = (sqrt(3)/2)*(uG-uB);
    H = atan2(y,x);
    S = sqrt(x.^2+y.^2);
    I = (uR + uG + uB)/3;
end

function uRGB = HSI2uRGB(HSI)
    H = HSI(:, :, 1);
    S = HSI(:, :, 2);
    I = HSI(:, :, 3);
    x = S.*cos(H);
    y = S.*sin(H);
    uR = I + 2/3*x;
    uG = I - 1/3*x + 1/sqrt(3)*y;
    uB = I - 1/3*x - 1/sqrt(3)*y;
    uRGB = cat(3, uR, uG, uB);
end