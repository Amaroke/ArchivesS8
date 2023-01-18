function test_fft(sample_rate, F1, F2) 
% Composantes fréquentielles d'un signal 
% ----------------------------------
% Construction d'un signal bruité échantilloné à sample_rate hz 
% contenant deux fréquences pures F1 et F2 Hz. 
figure; 
t = [0:1/sample_rate:0.6]; 
N = length(t); % nombre d'échantillons 
s = sin(2*pi*F1*t) + sin(2*pi*F2*t+pi/4); 
subplot(2,2,1); plot(t,s); 
y = fft(s); 
f = sample_rate*(0:(N-1))/N; % vecteur de fréquences associé 
n = length(f); 
subplot(2,2,2); plot(f,abs(y(1:n))); 
sb = s + normrnd(0,0.1,1,N); % on ajoute un peu de bruit (n'hésitez pas à tester avec + de 0.1) 
subplot(2,2,3); plot(t,sb); 
y = fft(sb); 
f = sample_rate*(0:(N-1)/2)/N; 
n = length(f); 
subplot(2,2,4); plot(f,abs(y(1:n))); 
end 