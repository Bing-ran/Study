
创建git版本管理：
	git init

查看分支：
	git branch

创建分支：
	git branch xxx

切换分支：
	git checkout 分支名

合并分支：
	git merge 要合并的分支

	注意：切换分支在合并

删除分支：
	git branch -d 分支名称

添加远程连接（别名）
	git remote add origin 给远程仓库写别名

向远程推送代码
	git push -u origin 分支

克隆仓库代码：
	git clone 远程仓库地址（内部实现 git remote add origin 原厂仓库地址）

切换分支：
	git checkout 分支名


拉去远程仓库代码：
	git pull origin 远端分支

	==》
	git fetch origin 分支名
	git merge origin/分支名


优化git提交记录（使记录更简洁）
	rebase

1、 提交1 提交2  提交3 ... 提交n
	git rebase -i 版本号 				合并当前版本到指定版本号及之间的提交记录
	git rebase -i head~3 				合并最近的3条记录  --> s --> xxxx

	注意：合并记录时不要合并已经push到仓库的记录

2、 master 	提交1 提交2 提交3 提交5
	dev  				提交4

	git checkout dev
	git rebase 
	git checkout master
	git merge dev 

3、 git pull origin dev 
	==>
	git fetch origin/dev 
	git rebase origin/dev

注意事项：
	执行git rebase时产生冲突 --> 解决冲突 --> git add 冲突文件 --> git rebase


git remote add origin git@github.com:Bing-ran/Linux-.git
git branch -M main
git push -u origin main


快速解决冲突：
	beyond compare

在git中配置		--local 只在当前工程有效
	git config --local merge.tool bc3		（bc3 别名）
	git config --local mergetool.path bc3 'xxxx'
	git config --local mergetool.keepBackup false
使用beyyong compare解决冲突
	git mergetool


打标签：
	现在本地打标签：git tag -a v1 -m "第一版"
				   git push origin --tags

删除分支：
	git branch -d 分支名

创建并切换到分支：
	git checkout -b dev

协同开发：gitflow流
	1、 git cone 地址
	2、 git checkout dev
	3、 git checkout -b ranbing

	在 ranbing 分支了

	开发...

	完毕...

	// 应该在网页上发起review，review通过后会自动合并，因此不再需要开发者合并（下一步就省略了）。
	{
		git push origin ranbing

		// reviews
		pull/merge requset
	}


