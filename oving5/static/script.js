function compileCode() {
    const code = document.getElementById('code').value;
    fetch('/compile', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({code: code}),
    })
        .then(response => response.json()) // Parses the JSON string in the response
        .then(data => {
            // Split the output by newline, prepend "➜ ~ " to each line, and join back with newline
            const formattedOutput = data.output.split('\n').map(line => "➜ ~ " + line).join('\n');

            // Extract the 'output' value, apply formatting, and replace it in the 'output' element
            document.getElementById('output').textContent = formattedOutput;
        })
        .catch((error) => {
            console.error('Error:', error);
            document.getElementById('output').textContent = 'Error: ' + error;
        });
}
