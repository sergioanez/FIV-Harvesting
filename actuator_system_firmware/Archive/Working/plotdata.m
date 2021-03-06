clear 
clc
close all
w = 0.1;
encoder_raw2 = [];
input_raw = cell2mat(table2cell(readtable('input_43.csv')));
encoder_raw = cell2mat(table2cell(readtable('encoder_43.csv',detectImportOptions('encoder_50.csv'))));

t_shift = input_raw(1,1);
conv2mm = 0.1/8;

input_y = input_raw(:,2) * conv2mm;
output_y = encoder_raw(:,3) * conv2mm;
input_v = input_raw(:,4);
input_t = input_raw(:,1)-t_shift;
output_t = encoder_raw(:,1)-t_shift;

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
plot(output_t, output_y,'--', input_t, input_y);
xlabel('Time (s)')
ylabel('Position (mm)')
legend({'Output','Input'})
title('System Tracking of Sum of 3 Sine Waves')
xlim([0 10])
