# PyHP - Python Hypertext Processor
PyHP is a hypertext processor for the Python language written in C. **CURRENTLY IN DEVELEOPMENT**.

## Demo
Consider the following document:
```php
<table>
<tr>
  <th colspan="3" class="pyhp-h1">PyHP Version <?print(__pyhp_info__["Version_PyHP"])?> Information</th>
</tr>
<tr>
  <td colspan="3" class="pyhp-h2">Information</td>
</tr>
<?
for i in __pyhp_info__.keys():
    print("<tr><td class='pyhp-name'>" + i + "</td><td colspan='2'>" + __escape_html__(__pyhp_info__[i]) + "</td></tr>")
?>
<tr>
  <td colspan="3" class="pyhp-h2">Environmental Variables</td>
</tr>
<?
for env in __ENVIRONMENTAL_VARIABLES__:
    print("<tr><td class='pyhp-name'>" + env.partition("=")[0] + "</td><td colspan='2'>" + env.partition("=")[2] + "</td></tr>")
?>
<tr>
  <td colspan="3" class="pyhp-h2">GET Params</td>
</tr>
<?
req_meth = __get_env__("REQUEST_METHOD")
?>
<?
if req_meth == "GET":
    if len(__get__) > 0:
        for get_param in __get__:
            print("<tr><td class='pyhp-name'>" + get_param + "</td><td colspan='2'>" + __get__[get_param][0] + "</td></tr>")
    else:
        print("<tr><td colspan='3'>None</td></tr>")
else:
    print("<tr><td colspan='3'>None, request method not GET</td></tr>")
?>
<tr>
  <td colspan="3" class="pyhp-h2">POST Params</td>
</tr>
<?
if req_meth == "POST":
    if len(__post__) > 0:
        for post_param in __post__:
            print("<tr><td class='pyhp-name'>" + post_param + "</td><td colspan='2'>" + __post__[post_param][0] + "</td></tr>")
    else:
        print("<tr><td colspan='3'>None</td></tr>")
else:
    print("<tr><td colspan='2'>None, request method not POST</td></tr>")
?>
</table>

```
Run this through PyHP, whether the CLI or socket, and you'll get something that looks like this:
```html
<table>
<tr>
  <th colspan="3" class="pyhp-h1">PyHP Version 1.0.0 Information</th>
</tr>
<tr>
  <td colspan="3" class="pyhp-h2">Information</td>
</tr>
<tr><td class='pyhp-name'>Version_PyHP</td><td colspan='2'>1.0.0</td></tr><tr><td class='pyhp-name'>Version_C</td><td colspan='2'>C11</td></tr><tr><td class='pyhp-name'>Compilation_Date</td><td colspan='2'>May 26 2023 13:49:10</td></tr><tr><td class='pyhp-name'>IS_FCGI</td><td colspan='2'>1</td></tr>
<tr>
  <td colspan="3" class="pyhp-h2">Environmental Variables</td>
</tr>
<tr><td class='pyhp-name'>FCGI_ROLE</td><td colspan='2'>RESPONDER</td></tr><tr><td class='pyhp-name'>QUERY_STRING</td><td colspan='2'></td></tr><tr><td class='pyhp-name'>REQUEST_METHOD</td><td colspan='2'>GET</td></tr><tr><td class='pyhp-name'>CONTENT_TYPE</td><td colspan='2'>application/x-www-form-urlencoded</td></tr><tr><td class='pyhp-name'>CONTENT_LENGTH</td><td colspan='2'>0</td></tr><tr><td class='pyhp-name'>SCRIPT_NAME</td><td colspan='2'>/info.pyhp</td></tr><tr><td class='pyhp-name'>REQUEST_URI</td><td colspan='2'>/info.pyhp</td></tr><tr><td class='pyhp-name'>DOCUMENT_URI</td><td colspan='2'>/info.pyhp</td></tr><tr><td class='pyhp-name'>DOCUMENT_ROOT</td><td colspan='2'>/var/www/html</td></tr><tr><td class='pyhp-name'>SERVER_PROTOCOL</td><td colspan='2'>HTTP/1.1</td></tr><tr><td class='pyhp-name'>REQUEST_SCHEME</td><td colspan='2'>http</td></tr><tr><td class='pyhp-name'>GATEWAY_INTERFACE</td><td colspan='2'>CGI/1.1</td></tr><tr><td class='pyhp-name'>SERVER_SOFTWARE</td><td colspan='2'>nginx/1.18.0</td></tr><tr><td class='pyhp-name'>REMOTE_ADDR</td><td colspan='2'>127.0.0.1</td></tr><tr><td class='pyhp-name'>REMOTE_PORT</td><td colspan='2'>40592</td></tr><tr><td class='pyhp-name'>REMOTE_USER</td><td colspan='2'></td></tr><tr><td class='pyhp-name'>SERVER_ADDR</td><td colspan='2'>127.0.0.1</td></tr><tr><td class='pyhp-name'>SERVER_PORT</td><td colspan='2'>8010</td></tr><tr><td class='pyhp-name'>SERVER_NAME</td><td colspan='2'>_</td></tr><tr><td class='pyhp-name'>REDIRECT_STATUS</td><td colspan='2'>200</td></tr><tr><td class='pyhp-name'>REQUEST_URI</td><td colspan='2'>/info.pyhp</td></tr><tr><td class='pyhp-name'>SCRIPT_FILENAME</td><td colspan='2'>/var/www/html/info.pyhp</td></tr><tr><td class='pyhp-name'>REQUEST_BODY</td><td colspan='2'></td></tr><tr><td class='pyhp-name'>HTTP_HOST</td><td colspan='2'>localhost:8010</td></tr><tr><td class='pyhp-name'>HTTP_USER_AGENT</td><td colspan='2'>curl/7.81.0</td></tr><tr><td class='pyhp-name'>HTTP_ACCEPT</td><td colspan='2'>*/*</td></tr><tr><td class='pyhp-name'>HTTP_CONTENT_LENGTH</td><td colspan='2'>0</td></tr><tr><td class='pyhp-name'>HTTP_CONTENT_TYPE</td><td colspan='2'>application/x-www-form-urlencoded</td></tr>
<tr>
  <td colspan="3" class="pyhp-h2">GET Params</td>
</tr>

<tr><td colspan='3'>None</td></tr>
<tr>
  <td colspan="3" class="pyhp-h2">POST Params</td>
</tr>
<tr><td colspan='2'>None, request method not POST</td></tr>
</table>
```
Cool, huh?

## Installation
Currently, no binaries are available to download, so you must build from source.

#### Install required libraries
```shell
sudo apt install libpython3.10 libpython3.10-dev
```
#### Download
```shell
git clone https://github.com/Sid220/PyHP.git
```
#### Build
```shell
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/path/to/ninja -G Ninja -S /path/to/git_clone -B /path/to/where/you/want/build
```
#### Run (optional)
```shell
/path/to/where/you/want/build/PyHP
```
#### Configure with NGINX
First start the socket:
```shell
chmod +x /path/to/git_clone/start.sh
/path/to/git_clone/start.sh
```
Then configure with NGINX. Here is some example configuration to get you started:
```nginx
server
{
	root /var/www/html;
	listen 8010 default_server;
	listen [::]:8010 default_server;
	server_name _;
	location /
	{
		# First attempt to serve request as file, then
		# as directory, then fall back to displaying a 404.
		try_files $uri $uri/ =404;
	}

	location ~ \.pyhp$
	{
		fastcgi_pass unix:/tmp/phyp-fcgi.sock;
		include fastcgi_params;
		fastcgi_param REQUEST_URI $uri;
		fastcgi_param SCRIPT_FILENAME $request_filename;
		fastcgi_param REQUEST_BODY $request_body;
	}
}
```
Now open up [http://localhost:8010/yourfile.pyhp](http://localhost:8010/yourfile.pyhp) to see some magic!
## ToDo
PyHP is by no means finished, here's what we hope to do:

- [ ] Speed up add_content and add_header methods, or find a better way to output
- [ ] Take other files through CLI
- [ ] Improve/create indentation fixer to remove extra indentation in files
- [ ] Add support for HTTP files
- [ ] Write Python documentation
- [ ] Write documentation for FastCGI and CLI
- [ ] Write NGINX guide
- [ ] More customization options
- [ ] Add support for custom headers from Python (`__header_add__` funtion)
