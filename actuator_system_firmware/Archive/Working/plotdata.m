clear 
clc
close all
w = 0.1;
encoder_raw2 = [];
input_raw = cell2mat(table2cell(readtable('input_test_0.csv')));
encoder_raw = cell2mat(table2cell(readtable('encoder_0.csv',detectImportOptions('encoder_0.csv'))));

t_shift = input_raw(1,1);

plot(input_raw(:,1)-t_shift, input_raw(:,2));
yyaxis right
plot(input_raw(:,1)-t_shift, input_raw(:,4));



figure 
plot(encoder_raw(:,1), encoder_raw(:,5), input_raw(:,1), input_raw(:,2));

legend({'Enc','In'})
