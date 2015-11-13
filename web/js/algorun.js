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
    var timer = $.timer(function () {
        o_editor.setValue(o_editor.getValue() + '.');
    });
    timer.set({time: 1000});
	var input_data = i_editor.getValue();
    if (input_data.trim() == ""){
        sweetAlert("Oops...", "Should you pass input to the computation?", "error");
    } else {
        timer.play();
        var start_time = Date.now() | 0;
        o_editor.setValue('please wait while computation is running...');
        $('#run_button').prop('disabled', true);
	   var jqxhr = $.post( "/v1/run", { input: input_data })
	   .done(function(data,textStatus,jqXHR) {
           try {
                json = $.parseJSON(data);
                o_editor.getSession().setMode("ace/mode/json");
                o_editor.setValue(json);
           } catch (e) {
                o_editor.setValue(data);
           } finally {
                o_editor.gotoLine(1);
                $('#run_button').prop('disabled', false);
                var run_time = ((Date.now() | 0) - start_time) / 1000.0;
                if(data.lastIndexOf("Are you sure", 0) != 0){
                  swal({   title: "Computation Run Time",   text: run_time + " seconds",   timer: 2000,   showConfirmButton: false });  
                } 
                timer.stop();
           }
       })
	   .fail(function() {
           o_editor.setValue('An error occured!');
           $('#run_button').prop('disabled', false);
           timer.stop();
       });
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
