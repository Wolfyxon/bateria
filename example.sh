./bateria \
--target 30 \
--command "notify-send 'Battery 30%' 'Connect charger soon'" \
\
--target 25 \
--command "notify-send --urgency=critical 'Battery 25%' 'Connect charger'" \
\
--target 10 \
--command "notify-send --urgency=critical 'Battery 10%' 'I said CONNECT CHARGER'" \
\
--target 1 \
--command "notify-send --urgency=critical '1%. Goodbye cruel world'"
