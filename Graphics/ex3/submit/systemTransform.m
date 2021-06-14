function dp = systemTransform(cp, b1, b2, b3, metatopish)
%SYSTEMTRANSFORM Transform the system to a new origin and axes

    % Theloume na vroume ton pinaka L tou metasxhmatismou, apo ta b1,b2,b3
    %L = [b1 b2 b3];
    % tha vroume kateutheian ton antistrofo, pou isoutai me ton anastrofo
    Linv = [b1 b2 b3]';
    
    % Se omogeneis suntetagmenes
    Th = [Linv, -Linv*metatopish ; 0 0 0 1];
    
    cp_h = [cp ; ones(1, size(cp, 2))];
    dp_h = Th * cp_h;
    dp = dp_h(1:3, :); %discard last line
    
end
