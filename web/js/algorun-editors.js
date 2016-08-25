

function addElement(element, options) {
	newElement = document.createElement(element)
	options_arr = options.split('&')
	for(i in options_arr){
		option = options_arr[i]
		arr = option.split("=")
		attr = arr[0].trim()
		value = arr[1].trim()
		newElement.setAttribute(attr, value)
	}
	return newElement;
}

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
	document.getElementById(id).style.fontSize='15px';
	// //editor.getSession().setMode("ace/mode/json");
	if(type == 'output'){
		editor.setReadOnly(true)
	}
}

function add_editor(id, type) {
	if(type == 'input'){
		tabs = 'input_tabs'
		list = 'input_list'
		classname = 'i_editor'
	} else {
		tabs = 'output_tabs'
		list = 'output_list'
		arr = id.split("=")
		id = arr[0]
		src = arr[1]
		classname = 'o_editor'
	}
	editor = document.getElementById(tabs)
	ul = document.getElementById(list)

	li = document.createElement('li')

	a = addElement('a', 'data-toggle=tab&href=#' + id)
	try{
		a.setAttribute('id', src)
	} catch(err) {
	}
	a.innerHTML = id

	li.appendChild(a)
	ul.appendChild(li)

	div_class = 'class=tab-pane fade'
	div = addElement('div', 'id=' + id + "&class=tab-pane fade")
	pre = addElement('pre', 'id=' + id + "_editor&style=position: relative; top: 0; right: 0; bottom: 0; left: 100; height: 400px; align: left;")
	div.appendChild(pre)
	editor.appendChild(div)

	formatEditor(id + "_editor", type)
}

formatEditor('input_editor', 'input')
formatEditor('output_editor', 'output')

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

	for(i in input_arr){	
		input_name = input_arr[i]
		if(i == 0){
			a = document.getElementById('input_a')
			a.innerHTML = input_name
			a.setAttribute('href', '#' + input_name)

			div = document.getElementById('input')
			div.setAttribute('id', input_name)

			pre = document.getElementById('input_editor')
			id = input_name + '_editor'
			pre.setAttribute('id', id)

			formatEditor(id, 'input')
		}
		else{
			add_editor(input_name, 'input')
		}
	}


	for(i in output_arr){	
		output = output_arr[i]
		arr = output.split("=")
		output_name = arr[0]
		output_src = arr[1]
		if(i == 0){
			a = document.getElementById('output_a')
			a.innerHTML = output_name
			a.setAttribute('id', output_src)
			a.setAttribute('href', '#' + output_name)

			div = document.getElementById('output')
			div.setAttribute('id', output_name)

			pre = document.getElementById('output_editor')
			id = output_name + '_editor'
			pre.setAttribute('id', id)

			formatEditor(id, 'output')
		}
		else{
			add_editor(output, 'output')
		}
	}
}, 'text');

