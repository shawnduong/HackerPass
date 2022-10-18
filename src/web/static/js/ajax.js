$("#user-update-name").submit(function()
{
	if ($("#user-update-name-field").val() == "")  return;

	$.ajax(
	{
		url: "/ajax/user/update/name",
		type: "POST",
		data: $("#user-update-name").serialize(),
		success: function(response)
		{
			if (response["Status"] == "Name updated.")
			{
				$("#user-name").html($("#user-update-name-field").val());
				$("#user-update-name-field").val("");
			}
		}
	});

	return false;
});

$("#user-update-email").submit(function()
{
	if ($("#user-update-email-field").val() == "")  return;

	$.ajax(
	{
		url: "/ajax/user/update/email",
		type: "POST",
		data: $("#user-update-email").serialize(),
		success: function(response)
		{
			if (response["Status"] == "Email updated.")
			{
				$("#user-email").html($("#user-update-email-field").val());
				$("#user-update-email-field").val("");
			}
		}
	});

	return false;
});
