function saveTextAsFile()
{
	id = $('#output_list').children('.active').children('a').attr('href')
	editor_name = $(id).children('pre').attr('id')
	o_editor = ace.edit(editor_name)
	var textToWrite = o_editor.getValue();
    if(textToWrite.trim() == ""){
        sweetAlert("Oops...", "It seems like you have not run the computation", "error");
        return;
    }
	var textFileAsBlob = new Blob([textToWrite], {type:'text/plain'});
	var fileNameToSaveAs = "computation result";

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
		downloadLink.onclick = destroyClickedElement;
		downloadLink.style.display = "none";
		document.body.appendChild(downloadLink);
	}

	downloadLink.click();
}
$("#download_file").click(function() {
	saveTextAsFile();
});