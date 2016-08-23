$(function() {
    $('.navclick').click(function(e) {
        $('.navbar ul li.active').removeClass('active');
        var $this = $(this);
        if (!$this.parent().hasClass('active')) {
            $this.parent().addClass('active');
        }
        if (location.pathname.replace(/^\//,'') == this.pathname.replace(/^\//,'') && location.hostname == this.hostname) {
            var target = $(this.hash);
            target = target.length ? target : $('[name=' + this.hash.slice(1) +']');
            if (target.length) {
                $('html,body').animate({
                    scrollTop: target.offset().top - 60
                }, 1000);
                return false;
            }
        }
    });
});