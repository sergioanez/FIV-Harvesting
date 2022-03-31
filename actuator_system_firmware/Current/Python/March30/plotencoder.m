
clear 
clc
close all

encoder_raw = cell2mat(table2cell(readtable('encoder_0.csv',detectImportOptions('encoder_0.csv'))));

t_shift = encoder_raw(1,1);
conv2mm = 0.1/8;

output_y = encoder_raw(:,5) * conv2mm;
output_t = encoder_raw(:,1)-t_shift;


figure
plot(output_t, output_y,'*');

xlabel('Time (s)')
ylabel('Position (mm)')
legend({'Output','Input','Laser'})
title('System Tracking of 10 mm Amplitude Sine Wave with 0.5 Hz Frequency')
