#include <bits/stdc++.h>
using namespace std;

struct Slice
{
	int size;
	char *data;
};
struct node
{
	int cnt[52] = {0};
	node *ptr[52] = {NULL};
	char *val = NULL;
};

class kvstore
{

private:
	node *root = (node *)malloc(sizeof(node));

public:
	bool get(Slice &key, Slice &value)
	{
		node *cur = root;
		for (int i = 0; i < key.size; i++)
		{
			if (key.data[i] <= 'Z' && cur->cnt[key.data[i] - 'A'])
			{
				cur = cur->ptr[key.data[i] - 'A'];
			}
			else if (key.data[i] > 'Z' && cur->cnt[key.data[i] - 'a' + 26])
			{
				cur = cur->ptr[key.data[i] - 'a' + 26];
			}
			else
			{
				cur = NULL;
				break;
			}
		}
		if (!cur)
			return false;
		value.size = strlen(cur->val);
		value.data = (char *)malloc((value.size + 1) * 8);
		strcpy(value.data, cur->val);
		return true;
	}

	bool put(Slice &key, Slice &value)
	{
		node *cur = root;
		for (int i = 0; i < key.size; i++)
		{
			if (key.data[i] <= 'Z')
			{
				cur->cnt[key.data[i] - 'A']++;
				if (!cur->ptr[key.data[i] - 'A'])
					cur->ptr[key.data[i] - 'A'] = (node *)malloc(sizeof(node));
				cur = cur->ptr[key.data[i] - 'A'];
			}
			else
			{
				cur->cnt[key.data[i] - 'a' + 26]++;
				if (!cur->ptr[key.data[i] - 'a' + 26])
					cur->ptr[key.data[i] - 'a' + 26] = (node *)malloc(sizeof(node));
				cur = cur->ptr[key.data[i] - 'a' + 26];
			}
		}
		cur->val = (char *)realloc(cur->val, (value.size + 1) * 8);
		strcpy(cur->val, value.data);
		return true;
	}

	bool delt(node *cur, Slice &key, int i)
	{
		if (i == key.size)
		{
			free(cur->val);
			return true;
		}
		if (key.data[i] <= 'Z' && cur->cnt[key.data[i] - 'A'])
		{
			cur = cur->ptr[key.data[i] - 'A'];
			if (!delt(cur, key, i + 1))
				return false;
			cur->cnt[key.data[i] - 'A']--;
		}
		else if (key.data[i] > 'Z' && cur->cnt[key.data[i] - 'a' + 26])
		{
			cur = cur->ptr[key.data[i] - 'a' + 26];
			if (!delt(cur, key, i + 1))
				return false;
			cur->cnt[key.data[i] - 'a' + 26]--;
		}
		else
			return false;
		return true;
	}

	bool del(Slice &key)
	{
		return delt(root, key, 0);
	}

	bool get(int N, Slice &key, Slice &value)
	{
		char temp[65];
		int j = 0;
		node *cur = root;
		while (N)
		{
			int sum = 0;
			for (int i = 0; i < 52; i++)
			{
				sum += cur->cnt[i];
				if (sum >= N)
				{
					N -= sum - cur->cnt[i];
					temp[j++] = i < 26 ? 'A' + i : 'a' + i - 26;
					cur = cur->ptr[i];
					break;
				}
			}
			if (sum < N)
				return false;
		}
		temp[j] = '\0';
		key.size = strlen(temp);
		key.data = (char *)malloc((key.size + 1) * 8);
		strcpy(key.data, temp);
		value.size = strlen(cur->val);
		value.data = (char *)malloc((value.size + 1) * 8);
		strcpy(value.data, cur->val);
		return true;
	}

	bool delt(int N, node *cur)
	{
		if (!N)
		{
			free(cur->val);
			return true;
		}
		int sum = 0;
		for (int i = 0; i < 52; i++)
		{
			sum += cur->cnt[i];
			if (sum >= N)
			{
				N -= sum - cur->cnt[i];
				cur = cur->ptr[i];
				if (!delt(N, cur))
					return false;
				cur->cnt[i]--;
				return true;
			}
		}
		if (sum < N)
			return false;
	}

	bool del(int N)
	{
		return delt(N, root);
	}
};