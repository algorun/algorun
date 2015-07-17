/*
    Algorun turns command-line algorithms into ready-to-use web enabled 
    containers - Copyright (C) 2015 Thibauld Favre <tfavre@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
$("#run_button").click(function() {
	var input_data = i_editor.getValue();
    if (input_data.trim() == ""){
        sweetAlert("Oops...", "Should you pass input to the computation?", "error");
    } else {
	   var jqxhr = $.post( "/do/run", { input: input_data })
	   .done(function(data,textStatus,jqXHR) {o_editor.setValue(data);o_editor.gotoLine(1);})
	   .fail(function() {o_editor.setValue('An error occured!');});
    }
});
$("#populate_input").click(function() {
    $.get("/algorun_info/input_example.txt", function(data){
        i_editor.setValue(data);
        i_editor.gotoLine(1);
    });
});
$("#reset_computation").click(function() {
	i_editor.setValue('');
    o_editor.setValue('');
});
$("#change_param").click(function(){
    var def_val = $("#def_val").html();
    if(def_val == "no parameter selected"){
        sweetAlert("Oops...", "Have you selected a parameter?", "error");
    } else {
        var new_val = $("#new_val").val();
        if(new_val == ""){
            sweetAlert("Oops...", "Did you forget to enter the new value?", "error");
        } else {
            var param = $("#param_menu_header").html();
            var req_body = {};
            req_body[param] = new_val;
            var jqxhr = $.post( "/do/config", req_body)
	                      .done(function(data,textStatus,jqXHR) {
                             if(data.substring(0, 6) == "Cannot"){
                                sweetAlert("I'm sorry...", "There is no such parameter!", "error");
                             } else {
                                 swal({
                                     title: "Parameter Changed!",
                                     type: "success",
                                     text: data,
                                     showConfirmButton: true });
                             }
                          })
	                      .fail(function() {sweetAlert("Oops...", "Unexpected error occured!", "error");})   
                    }
    }
});