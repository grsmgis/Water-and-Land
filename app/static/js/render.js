function render(initial){
    const button = document.querySelector('.render-btn');
    const i_options = {};
    const o_options = {};

    let i_perms = {
        type:'input',
        ncpath:document.getElementById("basin-input").value, 
        variable:document.getElementById("varname-input").value,
        date:document.getElementById("date-input").value,
        hour:document.getElementById("hour-input").value
    }
    let o_perms = {
        type:'output',
        ncpath:document.getElementById("basin-output").value, 
        variable:document.getElementById("varname-output").value,
        date:document.getElementById("date-output").value,
        hour:document.getElementById("hour-output").value
    }

    if (!initial) {
        o_options.body = JSON.stringify(o_perms);
        o_options.headers = {'Content-Type': 'application/json'};
        o_options.method = 'POST';

        i_options.body = JSON.stringify(i_perms);
        i_options.headers = {'Content-Type': 'application/json'};
        i_options.method = 'POST';
    }

    fetch('/render/', i_options).then((response) => response.blob()).then((blob) => {
		const image_url = URL.createObjectURL(blob);
		const image = document.querySelector('.render-input');
		image.src = image_url;
	});
    fetch('/render/', o_options).then((response) => response.blob()).then((blob) => {
		const image_url = URL.createObjectURL(blob);
		const image = document.querySelector('.render-output');
		image.src = image_url;
	});

}
