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
  input_data = {}
  name_list = []
  $('#input_list').children().each(function() {
    id = $(this).children().attr('href')
    input_name = $(id).attr('id')
    i_editor = ace.edit(input_name + "_editor")
    input_value = i_editor.getValue()
    input_data[input_name] = input_value
    name_list.push(input_name)
  });
  for(i in name_list){
    input_name = name_list[i]
    if(input_data[input_name].trim() == "") {
      alert("Please pass input")
      //sweetAlert("Oops...", "Should you pass input to the computation?", "error");
      return
    }
  }
  $.notify({message: "please wait while computation is running..."},
    {
        delay: 10000,
        placement: {
		from: "bottom",
		align: "center"
        },
        type: "success"
    });
  $('#run_button').prop('disabled', true);
	var jqxhr = $.post( "/v1/run", input_data)
	.done(function(data,textStatus,jqXHR) {
    try {
      json = $.parseJSON(data);
      for(key in json) {
      	id = "#" + key
      	var output_val = json[key]
      	output_field = $(id).children()
      	if(output_field.attr('class') == "output_image") {
      		output_field.attr('src', output_val)
      	} else {
      		editor_name = output_field.attr('id')
      		o_editor = ace.edit(editor_name)
      		o_editor.setValue(output_val)
      	}
      }
    } catch (e) {
      if(typeof data === 'string'){
        alert(data)
        $('#run_button').prop('disabled', false);
        $("#reset_computation").click()
      } else {
        currentTab = $("#output_list").children('.active')
        currentTab.removeClass('active')
        for(key in data){
          id = "#" + key
          tab = ""
          $('#output_list').children().each(function() {
            if($(this).children().attr('href') == id){
              tab = $(this)
            }
          })
          tab.addClass('active')
          var output_val = data[key]
          output_field = $(id).children()
          if(output_field.attr('class') == "output_image") {
            output_field.attr('src', "")
          	output_field.attr('src', output_val)
          } else {
          	editor_name = output_field.attr('id')
	          o_editor = ace.edit(editor_name)
	          o_editor.setValue(output_val)
	        }
          tab.removeClass('active')
        }
        currentTab.addClass('active')
      }
    } finally {
      o_editor.gotoLine(1);
      $('#run_button').prop('disabled', false);
      var run_time = "<div align='center'>" + jqXHR.getResponseHeader('Run-Time') + "</div>";
      $('.alert').remove()
      $.notify({message: run_time},
        {
          delay: 3000,
          placement: {
		        from: "bottom",
		        align: "center"
          },
          type: "success"
        });
    }
  })
	.fail(function() {
    o_editor.setValue('An error occured!');
    $('#run_button').prop('disabled', false);
    timer.stop();
  });
});

$("#populate_input").click(function() {
    if(v2_0) {
      input_list = $('#input_list')
      currentTab = input_list.children(".active")
      currentTab.removeClass('active')
      input_list.children().each(function (){
        $(this).addClass('active')
        editor = $(this).children().attr('href')
        id = $(editor).attr('id')
        var editor_name = id + "_editor"
        path = "/algorun_info/input_example/" + id + ".txt"
        $.get(path)
          .success(function(result) {
            i_editor = ace.edit(editor_name)
            i_editor.setValue(result);
            i_editor.gotoLine(1);
          })
          .error(function(jqXHR, textStatus, errorThrown) {
            i_editor = ace.edit(editor_name)
            i_editor.setValue("No input example provided!!")
            i_editor.gotoLine(1);
          });
        $(this).removeClass('active')
      });
      currentTab.addClass('active')
    } else {
      $.get("/algorun_info/input_example.txt", function(data){
        current_tab = $('#input_list').children('.active')
        id = current_tab.children('a').attr('href')
        editor_name = $(id).attr('id') + '_editor'
        i_editor = ace.edit(editor_name)
        i_editor.setValue(data);
        i_editor.gotoLine(1);
    });
    }
});
function launchFullScreen(element) {
  if(element.requestFullScreen) {
    element.requestFullScreen();
  } else if(element.mozRequestFullScreen) {
    element.mozRequestFullScreen();
  } else if(element.webkitRequestFullScreen) {
    element.webkitRequestFullScreen();
  }
}
$("#reset_computation").click(function() {
  currentTabs = $(".nav-tabs").children(".active")
  currentTabs.removeClass('active')
	$('.nav-tabs').children().each(function() {
      $(this).addClass('active')
      field_loc = $(this).children().attr("href")
      field = $(field_loc).children()
      if(field.attr('class') == "output_image") {
        field.attr('src', "")
      } else {
        editor_name = field.attr('id')
        editor = ace.edit(editor_name)
        editor.setValue("")
      }
      $(this).removeClass('active')
  });
  currentTabs.addClass('active')
    //launchFullScreen(document.getElementById("fullscreen"));
});
