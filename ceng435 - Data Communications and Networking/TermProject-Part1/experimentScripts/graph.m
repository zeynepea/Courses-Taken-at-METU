x=[20,40,50];
y=[41.1603,80.8184,101.1108];
e=[0.809630671523,1.10141507557,1.08417912754];
errorbar(x,y,e);
xlabel('Emulated Delay(ms)/+-5ms');
ylabel('End-to-End Delay(ms)');
title('Emulated Delay vs End-to-End Delay');
print('graph.png')