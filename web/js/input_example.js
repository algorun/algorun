
div = $('.work')
p = $('<p>File(s):</p>')
div.append(p)

$.get('/algorun_info/manifest.json')
	.success(function(result) {
		alert("working")
	})
	.error(function(jqXHR, textStatus, errorThrown){
		alert(errorThrown)
	})

$.get("/algorun_info/manifest.json", function(data) {
	alert('working')
	if(data['algo_input']){
		ul = $('ul')
		for(i in data['algo_input']){
			input = data['algo_input'][i].name

			li = $("<li></li>")
			filename = input + ".txt"
			filepath = '/algorun_info/input_example/' + filename
			a = $("<a href=" + filepath + ">" + filename + "</a>")
			li.append(a)
			ul.append(li)
		}

	} else {
		ul = $('ul')
		li = $("<li></li>")
		filename = "/algorun_info/input_example.txt"
		a = $("<a href=" + filename + ">" + filename + "</a>")
		li.append(a)
		ul.append(a)
	}
});