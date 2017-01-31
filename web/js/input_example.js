
div = $('.files')
p = $('<p>File(s):</p>')
div.append(p)

function addLink(filepath, input){
	$.get(filepath)
		.success(function(result) {
			filename = input + ".txt";
			a = $("<a href=" + filepath + ">" + filename + "</a>")
			li = $("#" + input)
			li.append(a)
		})
		.error(function(jqXHR, textStatus, errorThrown){
			filename = input + ".txt"
			a = $("<a href=/html/no_input.html>" + filename + '</a>')
			li = $("#" + input)
			li.append(a)
		});
}

$.get("/algorun_info/manifest.json", function(data) {
	data = JSON.parse(stripJsonComments(data));
	if(data['manifest_version'] == '2.0'){
		var ul = $('ul')
		for(i in data['algo_input']){
			var input = data['algo_input'][i].name
			li = $("<li id=" + input + "></li>")
			filename = input + ".txt"
			filepath = '/algorun_info/input_example/' + filename
			addLink(filepath, input)
			ul.append(li)
		}

	} else {
		$.get("/algorun_info/input_example.txt")
			.success(function(result){
				window.location = '/algorun_info/input_example.txt'
			})
			.error(function(jqXHR, textStatus, errorThrown) {
				window.location = '/html/no_input.html'
			})
	}
}, 'text');