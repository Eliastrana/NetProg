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
            let formattedOutput;
            // Check if there's an output or an error in the response
            if (data.output) {
                // Split the output by newline, prepend "➜ ~ " to each line, and join back with newline
                formattedOutput = data.output.split('\n').map(line => "➜ ~ " + line).join('\n');
            } else if (data.error) {
                // Handle error message similarly, in case you also want to format error messages
                formattedOutput = data.error.split('\n').map(line => "Error: " + line).join('\n');
            } else {
                // Fallback message in case neither output nor error is available
                formattedOutput = "No output or error message received.";
            }
            // Replace the content of the 'output' element with the formatted output or error message
            document.getElementById('output').textContent = formattedOutput;
        })
        .catch((error) => {
            console.error('Error:', error);
            // Handle fetch errors (e.g., network issues, invalid JSON response)
            document.getElementById('output').textContent = 'Fetch Error: ' + error.toString();
        });
}
