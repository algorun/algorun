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
$("#run_form").submit(function(event) {
	event.preventDefault();
	var input_data=$("#input_data").val();
	var jqxhr = $.post( "/do/run", { input: input_data })
	.done(function(data,textStatus,jqXHR) {$("#output_data").html(data);})
	.fail(function() {$("#output_data").val("An error occurred")});
});
$("#change_form").submit(function(event) {
	event.preventDefault();
});
$("#populate_input").click(function() {
	$("#input_data").val($("#input_example").text());
});
$("#reset_computation").click(function() {
	$("#input_data").val("");
	$("#output_data").val("");
});
