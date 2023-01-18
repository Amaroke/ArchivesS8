function exercice345() 
    sRGB = imread("monaloa.jpg");
    subplot(1,3,1),
    imshow(sRGB);
    subplot(1,3,2),
    [H,S,I] = getHSI(sRGB);
    I = tonecurve(I);
    HSI = cat(3, H, S, I);
    imshow(HSI2uRGB(HSI));
    subplot(1,3,3),
    [H,S,I] = getHSI(sRGB);
    S = normalise(S);
    S = tonecurve(S);
    I = normalise(I);
    I = tonecurve(I);
    HSI = cat(3, H, S, I);
    imshow(HSI2uRGB(HSI));
end

function y = tonecurve(x)
    y = logsig(5*(2*x-1));
end

function Y = normalise(X)
    Y = X-min(X,[], "all");
    Y = Y/max(Y, [], "all");
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