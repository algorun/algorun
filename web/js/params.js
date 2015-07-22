var params;
function configure_params(params) {
    if (jQuery.isEmptyObject(params)) {
        $("#params_window").empty();
        $("#params_window").html('<h5 align="center" style="color:orange;">No Specific Parameters Configuration</h5>');
    } else {
        $("#params_table").empty();
        var i = 1;
        var tabIndex = 0;
        var tabArray = new Array();
        $.each( params, function( key, value ) {    
            $("#params_table").append(parse("<div class='row' align='center' ><div class='col-md-2'>%s.</div><div class='col-md-5' align='left'>%s</div><div class='col-md-5' align='right'><a id='%s'></a></div></div>", i, key, key));
            $('#params_window').click(function(e) {
                e.stopPropagation();
            });
            $('#'+key).click(function(e) {
                e.stopPropagation();
                tabIndex = tabArray.indexOf(key);
            });
            shortcut.add("Alt+"+i,function() {
                if(!$('#params_window').data('open')) { 
                    $("#param_settings").click();
                }
                tabArray.forEach(function(i) {
                    $("#"+i).editable('hide');
                });
                $("#"+key).click();
            });
            tabArray[i] = key;
            i += 1;
            $.fn.editable.defaults.mode = 'inline';
            $('#'+key).editable({
                type: 'text',
                pk: 1,
                title: 'Enter username',
                highlight: '#00FF00',
                showbuttons: false,
                value: value,
                emptytext: value,
                defaultValue: value,
                onblur: 'submit',
                tpl: "<input type='text' style='width: 75px; text-align:center;'>",
                success: function(response, newValue) {
                    var req_body = {};
                    req_body[key] = newValue;
                    var jqxhr = $.post( "/do/config", req_body)
                    .done(function(data,textStatus,jqXHR) {
                        if(data.substring(0, 6) == "Cannot"){
                            sweetAlert("I'm sorry...", "There is no such parameter!", "error");
                            $("#"+key).html(value);
                        }
                    })
                    .fail(function() {
                        sweetAlert("Oops...", "Unexpected error occured!", "error");
                        $("#"+key).html(value);
                        });   
                    }
                }
            );
        });   
        shortcut.add("Tab",function() {
            if($('#params_window').data('open')) {
                $("#"+tabArray[(tabIndex)%(tabArray.length)]).editable('hide');
                $("#"+tabArray[(++tabIndex)%(tabArray.length)]).click();
            } else {
                $("#param_settings").click();
                $("#"+tabArray[(tabIndex)%(tabArray.length)]).editable('hide');
                $("#"+tabArray[(++tabIndex)%(tabArray.length)]).click();
            }
        });
        shortcut.add("Shift+Tab",function() {
            if($('#params_window').data('open')) { 
                if(tabIndex == 0 || tabIndex == 1){
                    tabIndex = 2;
                }
                $("#"+tabArray[(tabIndex)%(tabArray.length)]).editable('hide');
                $("#"+tabArray[(--tabIndex)%(tabArray.length)]).click();
            } else {
                $("#param_settings").click();
                if(tabIndex == 0 || tabIndex == 1){
                    tabIndex = 2;
                }
                $("#"+tabArray[(tabIndex)%(tabArray.length)]).editable('hide');
                $("#"+tabArray[(--tabIndex)%(tabArray.length)]).click();
            }
        });
    }
}
function parse(str) {
    var args = [].slice.call(arguments, 1),
        i = 0;

    return str.replace(/%s/g, function() {
        return args[i++];
    });
}
function update_ui(){
    $.getJSON( "/algorun_info/manifest.json", function( data ) {
	   $( "#algo_name" ).html( data["algo_name"] );
        $( "#algo_tab" ).html( data["algo_name"] );
	   $( "#about_algo_name" ).html( data["algo_name"] );
	   $( "#algo_title" ).html( data["algo_name"] );

	   $( "#short_description" ).html( data["short_description"] );
        
	   var authors = "";
	   for (var i = 0; i < data["authors"].length; i++) {
           if(i == 0){
               authors += data["authors"][i];
            } else {
                authors += ', ' + data["authors"][i];
            }
	   }
	   $( "#authors" ).html( authors );		
        
	   $( "#long_description" ).html( data["long_description"] );
        
	   $( "#algo_ref" ).attr( 'href', data["web_link"] );
        $( "#algo_ref" ).html( data["web_link"] );
        
        $( "#offline_command" ).html( data["offline_command"] );
        
        params = data["params"];
        configure_params(params);
    });
}
$('#reset_params').click(function(e) {
        e.stopPropagation();
        configure_params(params);
        var req_body = {};
        $.each( params, function( key, value ) {    
            req_body[key] = value;
        });
        var jqxhr = $.post( "/do/config", req_body)
           .done(function(data,textStatus,jqXHR) {
                 if(data.substring(0, 6) == "Cannot"){
                    sweetAlert("I'm sorry...", "There is no such parameter!", "error");
                    $("#"+key).html(value); 
                    } else{
                        swal({
                            title: "Reset to Defaults ..",
                            type: "success",
                            text: "Parameters successfully reset to defaults",
                            timer: 2000,
                            showConfirmButton: false });
                    }
              })
           .fail(function() {
                  sweetAlert("Oops...", "Unexpected error occured!", "error");
                  $("#"+key).html(value)
        });
});
update_ui();
$('.dropup.keep-open').on({
    "shown.bs.dropdown": function() { this.closable = false; },
    "click":             function() { this.closable = true; },
    "hide.bs.dropdown":  function() { return this.closable; }
});
$(document).ready(function() {

    $('#params_window').data('open', false);

    $('#param_settings').click(function() {
        if($('#params_window').data('open')) {
            $('#params_window').data('open', false);
        } else
            $('#params_window').data('open', true);
    });

    $(document).click(function() {
        if($('#params_window').data('open')) {
            $('#params_window').data('open', false);
        }
    });
});