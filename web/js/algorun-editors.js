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

function add_image(output){
	tab_list = $("#output_tabs")
	ul = $("#output_list")
	arr = output.split("=")
	id = arr[0]
	src = arr[1]
	if(first){
		li =  $("#output_list").children(".active")
		a = li.children()
		a.attr("href", "#" + id)
		a.text(id)
		a.attr('id', 'image')
		first = false
		div = $("#output")
		div.attr("id", id)
		old_editor = div.children()
		old_editor.replaceWith("<img class=output_image src=/images/imageOutput.png>")
	} else {
		li = $("<li></li>")
		a = $("<a data-toggle=tab href=#" + id + ">" + id + "</a>")
		a.attr('id', 'image')
		div = $("<div id=" + id + " class=tab-pane fade></div>")
		img = $("<img src=/images/imageOutput.png>")
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
		arr = id.split("=")
		id = arr[0]
		src = arr[1]
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
	}

	try{
		a.attr('id', src)
	} catch(err) {
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
        	input_string += name + "&"
        }
        input_string = input_string.slice(0, input_string.length - 1)
    }
    if(data['algo_output']){
        for(i in data['algo_output']){
        	name = data['algo_output'][i].name
        	filename = data['algo_output'][i].src
        	if(filename.includes('.')){
        		filename = filename.slice(0, filename.indexOf('.'))
        	}
        	output_string += name + '=' + filename + "&"
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
		input_name = input_arr[i]
		add_editor(input_name, 'input')
	}

	first = true;
	for(i in output_arr){	
		output_name = output_arr[i]
		output_src = output_name.split("=")[1]
		png = false
		if(output_src == 'image'){
			png = true
		}
		if(png){
			add_image(output_name)
		} else{
			if(first){
				formatEditor("output_editor", "output")
			}
			add_editor(output_name, 'output')
		}
	}
	// for(i in output_arr){	
	// 	output = output_arr[i]
	// 	arr = output.split("=")
	// 	output_name = arr[0]
	// 	output_src = arr[1]
	// 	if(i == 0){
	// 		a = document.getElementById('output_a')
	// 		a.innerHTML = output_name
	// 		a.setAttribute('id', output_src)
	// 		a.setAttribute('href', '#' + output_name)

	// 		div = document.getElementById('output')
	// 		div.setAttribute('id', output_name)

	// 		pre = document.getElementById('output_editor')
	// 		id = output_name + '_editor'
	// 		pre.setAttribute('id', id)

	// 		formatEditor(id, 'output')
	// 	}
	// 	else{
	// 		add_editor(output, 'output')
	// 	}
	// }
}, 'text');

