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
  id = $('#output_list').children('.active').children('a').attr('href')
  editor_name = $(id).children('pre').attr('id')
  o_editor = ace.edit(editor_name)
  var timer = $.timer(function () {
    o_editor.setValue(o_editor.getValue() + '.');
  });
  timer.set({time: 1000});
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
  timer.play();
  o_editor.setValue('please wait while computation is running...');
  $('#run_button').prop('disabled', true);
	var jqxhr = $.post( "/v1/run", input_data)
	.done(function(data,textStatus,jqXHR) {
    try {
      json = $.parseJSON(data);
      for(key in json) {
        editor = $(key).attr('href')
        editor_name = $(editor).href('id')
        o_editor = ace.edit(editor_name)
        o_editor.getSession().setMode("ace/mode/json");
        o_editor.setValue(json);
      }
    } catch (e) {
      if(typeof data === 'string'){
        o_editor.setValue(data)
      } else {
        for(key in data){
          id = "#"
          if(key.includes(".")){
          	arr = key.split(".")
            id = key[0]
            ext = key[1]
          } else {
            id += key
          }
          output_loc = $(id).attr('href')
          output_area = $(output_loc)
          if(ext == "png"){
          	image = output_area.children()
          	image.attr('src', data[key])
          } else {
          	editor_name = output_area.attr('id') + "_editor"
	        o_editor = ace.edit(editor_name)
	        o_editor.setValue(data[key])
	      }
        }
      }
      o_list = $('#output_list')
      o_list.children().removeClass('active')
      o_list.children(':first').addClass('active')
    } finally {
      o_editor.gotoLine(1);
      $('#run_button').prop('disabled', false);
      var run_time = "<div align='center'>" + jqXHR.getResponseHeader('Run-Time') + "</div>";
      $.notify({message: run_time},
        {
          delay: 3000,
          placement: {
		        from: "bottom",
		        align: "center"
          },
          type: "success"
        });
      timer.stop();
    }
  })
	.fail(function() {
    o_editor.setValue('An error occured!');
    $('#run_button').prop('disabled', false);
    timer.stop();
  });
});

$("#populate_input").click(function() {
    if(v1_4) {
      input_list = $('#input_list')
      input_list.children().removeClass('active')
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
      input_list.children(':first').addClass('active')
      input_list.children(':first').removeClass('active')
      input_list.children(':first').addClass('active')
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
	$('.nav-tabs').each(function() {
    $(this).children().removeClass('active')
    $(this).children().each(function () {
      id = $(this).children().attr('href')
      editor_name = $(id).attr('id') + "_editor"
      editor = ace.edit(editor_name)
      editor.setValue("")
    });
    $(this).children(":first").addClass("active")
    $(this).children(":first").removeClass("active")
    $(this).children(":first").addClass("active")
  });


    
    //launchFullScreen(document.getElementById("fullscreen"));
});
