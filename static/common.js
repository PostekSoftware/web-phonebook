function logout() {
    var form = document.createElement("form")
    form.style.display = "none"
    form.action = "/admin"
    form.method = "post"
    document.body.appendChild(form)
    var action = document.createElement("input")
    action.type = "hidden"
    action.name = "logout"
    action.value = "true"
    form.appendChild(action)
    form.submit();
    document.body.removeChild(form)
}
function admin() {
    var form = document.createElement("form")
    form.style.display = "none"
    form.action = "/admin"
    form.method = "get"
    document.body.appendChild(form)
    form.submit();
    document.body.removeChild(form)
}
function landing() {
    var form = document.createElement("form")
    form.style.display = "none"
    form.action = "/"
    form.method = "get"
    document.body.appendChild(form)
    form.submit();
    document.body.removeChild(form)
}