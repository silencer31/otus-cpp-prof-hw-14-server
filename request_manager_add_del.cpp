// �������� ������ ������������.
bool RequestManager::add_user(UserType user_type, const std::string& login,
	const std::string& sname, const std::string& fname, const std::string& patr)
{
	// ���������, ���� �� ��� ����� ����� � ����.
	if (check_login(login)) {
		return false;
	}


}