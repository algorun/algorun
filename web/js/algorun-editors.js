var first = false

function formatEditor(id, type){
	if(type == 'input'){
		theme='ace/theme/xcode'
	} else {
		theme='ace/theme/twilight'
	}
	editor = ace.edit(id);
	editor.setTheme(theme);
	editor.getSession().setTabSize(4);
	editor.getSession().setUseSoftTabs(true);
	editor.gotoLine(1)
	document.getElementById(id).style.fontSize='15px';
	if(type == 'output'){
		editor.setReadOnly(true)
	}
}

function add_image(id){
	tab_list = $("#output_tabs")
	ul = $("#output_list")
	if(first){
		li =  $("#output_list").children(".active")
		a = li.children()
		a.attr("href", "#" + id)
		a.text(id)
		first = false
		div = $("#output")
		div.attr("id", id)
		old_editor = div.children()
		old_editor.replaceWith("<img class=output_image src=/images/imageOutput.png>")
	} else {
		li = $("<li></li>")
		a = $("<a data-toggle=tab href=#" + id + ">" + id + "</a>")
		div = $("<div id=" + id + " class=tab-pane fade></div>")
		img = $("<img class=output_image src=/images/imageOutput.png>")
		div.append(img)
		tab_list.append(div)
		li.append(a)
		ul.append(li)
	}
}

function add_editor(id, type) {
	if(type == 'input'){
		tab_list = $("#input_tabs")
		ul = $("#input_list")
	} else {
		tab_list = $("#output_tabs")
		ul = $('#output_list')
	}

	if(first){
		li = ul.children(".active")
		a = li.children()
		a.attr('href', "#" + id)
		a.text(id)
		div = $("#" + type)
		pre = div.children()
		div.attr("id", id)
		pre.attr("id", id + "_editor")
		first = false
	} else{
		li = $("<li></li>")
		a = $("<a data-toggle=tab href=#" + id + ">" + id + "</a>")
		div = $("<div id=" + id + " class=tab-pane fade></div>")
		pre = $("<pre id=" + id + "_editor></pre>")
		pre.attr('style', 'position: relative; top: 0; right: 0; bottom: 0; left: 100; height: 400px; align: left;')
		div.append(pre)
		tab_list.append(div)
        $("#inputParams").append(li)
	}

	formatEditor(id + "_editor", type)
	li.append(a)
	ul.append(li)
}

formatEditor("input_editor", "input")

$.get( "../algorun_info/manifest.json", function( data ) {
	var input_string = ""
	var output_string = ""

	try {
        data = JSON.parse(stripJsonComments(data));
    } catch(err) {
        alert(err)
    }
    if(data["algo_input"]){
        for(i in data['algo_input']){
        	name = data['algo_input'][i].name
        	type = data["algo_input"][i].type
        	input_string += name + ":" + type + "&"
        }
        input_string = input_string.slice(0, input_string.length - 1)
    }
    if(data['algo_output']){
        for(i in data['algo_output']){
        	name = data['algo_output'][i].name
        	type = data['algo_output'][i].type
        	output_string += name + ':' + type + "&"
        }
        output_string = output_string.slice(0, output_string.length - 1);
    }

    if(input_string == ""){
    	input_arr = []
    } else {
    	input_arr = input_string.split("&")
    }
    if(output_string == ""){
    	output_arr = []
    } else {
    	output_arr = output_string.split("&")
    }

    first = true;
	for(i in input_arr){	
		input_element = input_arr[i]
		input_name = input_element.split(":")[0]
		input_type = input_element.split(":")[1]
		if(v2_0){
			if (first){
	        	$("#input1").html("<b>" + input_name + ":</b> " + input_type)
			}
			else{
				listElement = $("<li><b>" + input_name + ":</b> " + input_type + "</li>")
	        	$("#inputParams").append(listElement)
			}
		}
		add_editor(input_name, 'input')
	}

	first = true;
	for(i in output_arr){	
		output_element = output_arr[i]
		output_name = output_element.split(":")[0]
		output_type = output_element.split(":")[1]
		if(v2_0){
			if (first){
				$("#output1").html("<b>" + output_name + ":</b> " + output_type)
			}
			else{
				listElement = $("<li><b>" + output_name + ":</b> " + output_type + "</li>")
	        	$("#outputParams").append(listElement)
			}
		}
		if(output_type.includes("image")){
			add_image(output_name)
		} else{
			if(first){
				formatEditor("output_editor", "output")
			}
			add_editor(output_name, 'output')
		}
	}
}, 'text');

