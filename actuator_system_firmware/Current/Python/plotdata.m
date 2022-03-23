clear 
clc
close all
w = 0.1;
encoder_raw2 = [];
input_raw = cell2mat(table2cell(readtable('input_test_0.csv')));
encoder_raw = cell2mat(table2cell(readtable('encoder_0.csv',detectImportOptions('encoder_0.csv'))));

t_shift = input_raw(1,1);
conv2mm = 0.1/8;

input_y = input_raw(:,3) * conv2mm;
output_y = encoder_raw(:,5) * conv2mm;
input_v = input_raw(:,4);
input_t = input_raw(:,1)-t_shift;
output_t = encoder_raw(:,1)-t_shift;

plot(input_t, input_y);
yyaxis right
plot(input_t, input_v);



figure 
plot(output_t, output_y, input_t, input_y);

legend({'Enc','In'})
