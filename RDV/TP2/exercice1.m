function exercice1()
    sRGB = imread("shadows.jpg");
    uRGB = sRGB2uRBG(sRGB);
    HSI = uRGB2HSI(uRGB);
    figure;
    imshow(HSI2uRGB(HSI));
    drawnow();
end

function uRGB = sRGB2uRBG(sRGB)
    uRGB = double(sRGB)/255;
end

function HSI = uRGB2HSI(uRGB)
    uR = uRGB(:, :, 1);
    uG = uRGB(:, :, 2);
    uB = uRGB(:, :, 3);
    x = uR - 0.5*(uG + uB);
    y = (sqrt(3)/2)*(uG-uB);
    H = atan2(y,x);
    S = sqrt(x.^2+y.^2);
    I = (uR + uG + uB)/3;
    HSI = cat(3, H, S, I);
end

function compshow(im)
    H = im(:, :, 1);
    S = im(:, :, 2);
    I = im(:, :, 3);
    subplot(1,3,1),
    imshow(H, [0,2*pi]);
    subplot(1,3,2),
    imshow(S);
    subplot(1,3,3),
    imshow(I);
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


