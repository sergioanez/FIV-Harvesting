clear 
clc
close all
w = 0.1;
encoder_raw2 = [];
input_raw = cell2mat(table2cell(readtable('input_test_14.csv')));
encoder_raw = cell2mat(table2cell(readtable('encoder_57.csv',detectImportOptions('fin.csv'))));

t_shift = input_raw(1,1);

plot(input_raw(:,1)-t_shift, input_raw(:,2));
yyaxis right
plot(input_raw(:,1)-t_shift, input_raw(:,4));



% figure 
% plot(encoder_raw(:,1), encoder_raw(:,4), input_raw(:,1), input_raw(:,2));
% 
% legend({'Enc','In'})
