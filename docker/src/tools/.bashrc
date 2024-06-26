# ~/.bashrc: executed by bash(1) for non-login shells.

# Note: PS1 and umask are already set in /etc/profile. You should not
# need this unless you want different defaults for root.
# PS1='${debian_chroot:+($debian_chroot)}\h:\w\$ '
# umask 022

# You may uncomment the following lines if you want `ls' to be colorized:
# export LS_OPTIONS='--color=auto'
# eval "$(dircolors)"
# alias ls='ls $LS_OPTIONS'
# alias ll='ls $LS_OPTIONS -l'
# alias l='ls $LS_OPTIONS -lA'
#
# Some more alias to avoid making mistakes:
# alias rm='rm -i'
# alias cp='cp -i'
# alias mv='mv -i'
alias rc='vim ~/.bashrc && source ~/.bashrc'
alias stat='service --status-all'
alias init='service fcgiwrap start && service nginx start'
alias s='nginx -s reload'
alias t='nginx -t'
alias co='vim /etc/nginx/nginx.conf'
alias con='cd /var/www/html'
alias loge='cat /var/log/nginx/error.log | tail -n 1'
alias loga='cat /var/log/nginx/access.log | tail -n 1'
alias pbcopy="xclip -selection c"
alias pbpaste="xclip -selection c -o"
