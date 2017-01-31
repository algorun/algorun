
div = $('.files')
p = $('<p>File(s):</p>')
div.append(p)

function addLink(filepath, output){
	$.get(filepath)
		.success(function(result) {
			filename = output;
			a = $("<a href=" + filepath + ">" + filename + "</a>")
			li = $("#" + output)
			li.append(a)
		})
		.error(function(jqXHR, textStatus, errorThrown){
			filename = output
			a = $("<a href=/html/no_output.html>" + filename + '</a>')
			li = $("#" + output)
			li.append(a)
		});
}

$.get("/algorun_info/manifest.json", function(data) {
	data = JSON.parse(stripJsonComments(data));
	if(data['manifest_version'] == '2.0'){
		var ul = $('ul')
		for(i in data['algo_output']){
			var output = data['algo_output'][i]
			name = output.name
			li = $("<li id=" + name + "></li>")
			type = output.type
			if(type.includes("jpeg")){
				ext = '.jpg'
			} else if(type.includes('png')){
				ext = '.png'
			}
			else{
				ext = '.txt'
			}
			filename = name + ext
			filepath = '/algorun_info/output_example/' + filename
			addLink(filepath, name)
			ul.append(li)
		}

	} else {
		$.get("/algorun_info/output_example.txt")
			.success(function(result){
				window.location = '/algorun_info/output_example.txt'
			})
			.error(function(jqXHR, textStatus, errorThrown) {
				window.location = '/html/no_output.html'
			})
	}
}, 'text');
