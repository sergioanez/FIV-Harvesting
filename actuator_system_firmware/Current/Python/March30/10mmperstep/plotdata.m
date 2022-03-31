clear 
clc
close all
w = 0.1;
% encoder_raw2 = [];
input_raw = cell2mat(table2cell(readtable('input_test_-1.csv')));
encoder_raw = cell2mat(table2cell(readtable('encoder_0.csv',detectImportOptions('encoder_0.csv'))));

t_shift = input_raw(1,1);
conv2mm = 0.1/8;


input_y = input_raw(:,3) * conv2mm;
output_y = encoder_raw(:,5) * conv2mm;
input_v = input_raw(:,4);
input_t = input_raw(:,1)-t_shift;
output_t = encoder_raw(:,1)-t_shift;

laser_y = -1*encoder_raw(:,6)+81;

a = max(find(laser_y<-50));
tt_shift = output_t(a);
output_t = output_t - tt_shift;
input_t = input_t - tt_shift;
y_shift = input_y(1)-output_y(1);

output_y = output_y(output_t > 0 )
output_t = output_t(output_t > 0 ) 

figure
plot(output_t, output_y,'*', input_t, input_y);
hold on
plot(output_t,laser_y,'o');
xlabel('Time (s)')
ylabel('Position (mm)')
legend({'Output','Input','Laser'})
title('System Tracking of 10 mm Amplitude Sine Wave with 0.5 Hz Frequency')
%xlim([0 10])

figure
plot(output_t, output_y,'*');

xlabel('Time (s)')
ylabel('Position (mm)')
legend({'Output','Input','Laser'})
title('System Tracking of 10 mm Amplitude Sine Wave with 0.5 Hz Frequency')

