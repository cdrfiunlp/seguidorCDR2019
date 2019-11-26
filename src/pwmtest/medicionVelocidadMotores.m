%%
% Script que grafica la relación entre velocidad (frecuencia en Hz) de giro
% vs ciclo de trabajo de pwm (de 0 a 320)
clear all;clc;

%%
% Datos tomados con osciloscopio.
M1vel = [0 0 0 0 0 0 2.2 2.87 3.56 4.24 4.92 5.63 6.3 6.95 7.6 8.3 9 9.7 10.3 11 11.65 12.3 13 13.6 14.3 15 15.4 16.1 16.8 17.6 18.1 18.3];
M2vel = [0 0 0 0 0 0 2.13 2.81 3.46 4.12 4.75 5.4 6.05 6.7 7.3 7.95 8.55 9.2 9.8 10.4 11.1 11.7 12.3 13 13.6 14.3 15.1 15.7 16.3 16.6 17.3 18];
pwm   = 10:10:320;
% Gráfico de las rectas
% plot(pwm,M1vel,'o-');hold on;plot(pwm,M2vel,'o-');legend('M1vel','M2vel');grid on;
% xlabel('Ciclo de trabajo PWM [0-320], [0-7.4?]V');
% ylabel('Frecuencia de giro [Hz]');

% Conversión a velocidad lineal [m/s]. La velocidad angular sería
% 2.pi.Mvel[Hz]. Luego se multiplica por el radio de 16 mm (0.016 m): 
M1vel_lineal = 0.016*2*pi*M1vel;
M2vel_lineal = 0.016*2*pi*M2vel;
% Gráfico de las rectas en escala de velocidad lineal
% figure;plot(pwm,M1vel_lineal,'o-');hold on;plot(pwm,M2vel_lineal,'o-');legend('M1vel lineal','M2vel lineal');grid on;
% xlabel('Ciclo de trabajo PWM [0-320], [0-7.4?]V');
% ylabel('Velocidad tangencial [m/s]');

%% regresion lineal para cada velocidad [en RPM]
% Conversión a revoluciones por minuto [RPM]. Las RPM se obtienen a partir 
% de la frecuencia de giro como 60.M1vel[Hz].
M1vel_RPM = 60*M1vel;
M2vel_RPM = 60*M2vel;
% regresion lineal [en RPM] 
[rM1,mM1,bM1] = regression(pwm,M1vel_RPM);
[rM2,mM2,bM2] = regression(pwm,M2vel_RPM);
% aproximando pendientes y ordenada...
% mM1 = 3.9; bM1 = -133.5;
% mM2 = 3.8; bM2 = -129.5;
% Gráfico de las rectas en RPM
figure;plot(pwm,M1vel_RPM,'o-');hold on;plot(pwm,M2vel_RPM,'o-');
plot(pwm,mM1.*pwm+bM1);plot(pwm,mM2.*pwm+bM2);
legend('M1vel RPM','M2vel RPM','LR M1vel','LR M2vel');grid on;
xlabel('Ciclo de trabajo PWM [0-320], [0-7.4?]V');
ylabel('Velocidad [RPM]');
% del gráfico se ve que mejor utilizar puntos hasta pwm = 260 para hacer la
% recta
[rM1_2,mM1_2,bM1_2] = regression(pwm(1:end-5),M1vel_RPM(1:end-5));
[rM2_2,mM2_2,bM2_2] = regression(pwm(1:end-5),M2vel_RPM(1:end-5));
% Gráfico de las rectas corregidas a la zona lineal en RPM
figure;plot(pwm,M1vel_RPM,'o-');hold on;plot(pwm,M2vel_RPM,'o-');
plot(pwm,mM1_2.*pwm+bM1_2);plot(pwm,mM2_2.*pwm+bM2_2);
legend('M1vel RPM','M2vel RPM','LR M1vel','LR M2vel');grid on;
xlabel('Ciclo de trabajo PWM [0-320], [0-7.4?]V');
ylabel('Velocidad [RPM]');
% aproximando pendientes y ordenadas con el registro limitado...
% mM1 = 3.93; bM1 = -135.5;
% mM2 = 3.77; bM2 = -129;