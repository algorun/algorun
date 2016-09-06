function saveTextAsFile()
{
	link = $('#output_list').children('.active').children('a')
	id = link.attr('href')
	filename = $(id).attr('id')
	editor_field = $(id).children()
	if(editor_field.attr("class") == "output_image"){
		filepath = editor_field.attr("src")
		var downloadLink = document.createElement("a");
		downloadLink.download = filename;
		downloadLink.innerHTML = "Download File";
		downloadLink.href = filepath;
		if (window.webkitURL == null)
		{
			downloadLink.style.display = "none";
			document.body.appendChild(downloadLink);
		}
	} else {
		editor_name = editor_field.attr('id')
		o_editor = ace.edit(editor_name)
		var textToWrite = o_editor.getValue();
	    if(textToWrite.trim() == ""){
	        alert("Oops...please run computation first");
	        return;
	    }
		var textFileAsBlob = new Blob([textToWrite], {type:'text/plain'});
		var fileNameToSaveAs = filename;

		var downloadLink = document.createElement("a");
		downloadLink.download = fileNameToSaveAs;
		downloadLink.innerHTML = "Download File";

		if (window.webkitURL != null)
		{
			// Chrome allows the link to be clicked
			// without actually adding it to the DOM.
			downloadLink.href = window.webkitURL.createObjectURL(textFileAsBlob);
		}
		else
		{
			// Firefox requires the link to be added to the DOM
			// before it can be clicked.
			downloadLink.href = window.URL.createObjectURL(textFileAsBlob);
			downloadLink.style.display = "none";
			document.body.appendChild(downloadLink);
		}

	}
	
	
	downloadLink.click();
}
$("#download_file").click(function() {
	saveTextAsFile();
});