clear 
clc
close all
w = 0.1;
encoder_raw2 = [];
input_raw = cell2mat(table2cell(readtable('input_test_2.csv')));
laser_raw = cell2mat(table2cell(readtable('laser_2.csv',detectImportOptions('laser_0.csv'))));
encoder_raw = cell2mat(table2cell(readtable('encoder_2.csv',detectImportOptions('encoder_0.csv'))));

t_shift = input_raw(1,1);
conv2mm = 0.1/8;

input_y = input_raw(:,3) * conv2mm;
output_y = encoder_raw(:,5) * conv2mm;
input_v = input_raw(:,4);
input_t = input_raw(:,1)-t_shift;
output_t = encoder_raw(:,1)-t_shift;

laser_y = laser_raw(:,2)-83.3;
laser_t = laser_raw(:,1)-t_shift;

y_shift = input_y(1)-output_y(1);


plot(input_t, input_y);
xlabel('Time (s)')
ylabel('Position (mm)')
yyaxis right
plot(input_t, input_v);
ylabel('Delay (ms)')
legend({'Input (position)','Input (PWM delay)'})
title('Inputs')
xlim([0 5])

figure
plot(output_t, output_y,'*', input_t, input_y);
hold on
plot(laser_t,laser_y,'o');
xlabel('Time (s)')
ylabel('Position (mm)')
legend({'Output','Input','Laser'})
title('System Tracking of 10 mm Amplitude Sine Wave with 3 Hz Frequency')
xlim([0 10])

figure
plot(laser_t,laser_y,'o');
